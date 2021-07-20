/*
 * Copyright 2010-2021 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bx#license-bsd-2-clause
 */

#include <bx/timer.h>
#include <bx/handlealloc.h>
#include <bx/maputil.h>

#include <tinystl/allocator.h>
#include <tinystl/unordered_map.h>

#include <unordered_map>

#include <stdio.h>
#include <assert.h>

#include "catch/catch.hpp"

TEST_CASE("Handle_bench", "")
{	const uint32_t numElements   = 4<<10;
	const uint32_t numIterations = 16;

	//
	{
		int64_t elapsed = -bx::getHPCounter();

		for (uint32_t ii = 0; ii < numIterations; ++ii)
		{
			typedef tinystl::unordered_map<uint64_t, uint16_t> TinyStlUnorderedMap;
			TinyStlUnorderedMap map;
//			map.reserve(numElements);
			for (uint32_t jj = 0; jj < numElements; ++jj)
			{
				tinystl::pair<TinyStlUnorderedMap::iterator, bool> ok = map.insert(tinystl::make_pair(uint64_t(jj), uint16_t(jj) ) );
				assert(ok.second); BX_UNUSED(ok);
			}

			for (uint32_t jj = 0; jj < numElements; ++jj)
			{
				bool ok = bx::mapRemove(map, uint64_t(jj) );
				assert(ok); BX_UNUSED(ok);
			}

			assert(map.size() == 0);
		}

		elapsed += bx::getHPCounter();
		printf("      TinyStl: %15f\n", double(elapsed) );
	}

	///
	{
		int64_t elapsed = -bx::getHPCounter();

		for (uint32_t ii = 0; ii < numIterations; ++ii)
		{
			typedef std::unordered_map<uint64_t, uint16_t> StdUnorderedMap;
			StdUnorderedMap map;
			map.reserve(numElements);
			for (uint32_t jj = 0; jj < numElements; ++jj)
			{
				std::pair<StdUnorderedMap::iterator, bool> ok = map.insert(std::make_pair(uint64_t(jj), uint16_t(jj) ) );
				assert(ok.second); BX_UNUSED(ok);
			}

			for (uint32_t jj = 0; jj < numElements; ++jj)
			{
				bool ok = bx::mapRemove(map, uint64_t(jj) );
				assert(ok); BX_UNUSED(ok);
			}

			assert(map.size() == 0);
		}

		elapsed += bx::getHPCounter();
		printf("          STL: %15f\n", double(elapsed) );
	}

	///
	{
		int64_t elapsed = -bx::getHPCounter();

		for (uint32_t ii = 0; ii < numIterations; ++ii)
		{
			typedef bx::HandleHashMapT<numElements+numElements/2, uint64_t> HandleHashMap;
			HandleHashMap map;
			for (uint32_t jj = 0; jj < numElements; ++jj)
			{
				bool ok = map.insert(jj, uint16_t(jj) );
				assert(ok); BX_UNUSED(ok);
			}

			for (uint32_t jj = 0; jj < numElements; ++jj)
			{
				bool ok = map.removeByKey(uint64_t(jj) );
				assert(ok); BX_UNUSED(ok);
			}

			assert(map.getNumElements() == 0);
		}

		elapsed += bx::getHPCounter();
		printf("HandleHashMap: %15f\n", double(elapsed) );
	}

	extern void simd_bench();
	simd_bench();

	extern void math_bench();
	math_bench();

// assert(bx::kExitSuccess);
printf("\nsuccess:%d \n",bx::kExitSuccess);

}


// int main()
// {

// 	return bx::kExitSuccess;
// }

#include <bx/allocator.h>
#include <bx/rng.h>
#include <bx/simd_t.h>
#include <bx/timer.h>

#include <stdio.h>

static void flushCache()
{
	static uint32_t length = 1 << 26;
	static uint8_t* input  = new uint8_t[length];
	static uint8_t* output = new uint8_t[length];
	bx::memCopy(output, input, length);
}

typedef bx::simd128_t (*SimdRsqrtFn)(bx::simd128_t _a);

template<SimdRsqrtFn simdRsqrtFn>
void simd_rsqrt_bench(bx::simd128_t* _dst, bx::simd128_t* _src, uint32_t _numVertices)
{
	for (uint32_t ii = 0, num = _numVertices/4; ii < num; ++ii)
	{
		bx::simd128_t* ptr = &_src[ii*4];
		bx::simd128_t tmp0 = bx::simd_ld(ptr + 0);
		bx::simd128_t tmp1 = bx::simd_ld(ptr + 1);
		bx::simd128_t tmp2 = bx::simd_ld(ptr + 2);
		bx::simd128_t tmp3 = bx::simd_ld(ptr + 3);
		bx::simd128_t rsqrt0 = simdRsqrtFn(tmp0);
		bx::simd128_t rsqrt1 = simdRsqrtFn(tmp1);
		bx::simd128_t rsqrt2 = simdRsqrtFn(tmp2);
		bx::simd128_t rsqrt3 = simdRsqrtFn(tmp3);

		ptr = &_dst[ii*4];
		bx::simd_st(ptr + 0, rsqrt0);
		bx::simd_st(ptr + 1, rsqrt1);
		bx::simd_st(ptr + 2, rsqrt2);
		bx::simd_st(ptr + 3, rsqrt3);
	}
}

void simd_bench_pass(bx::simd128_t* _dst, bx::simd128_t* _src, uint32_t _numVertices)
{
	const uint32_t numIterations = 10;

	{
		int64_t elapsed = 0;
		for (uint32_t test = 0; test < numIterations; ++test)
		{
			flushCache();
			elapsed += -bx::getHPCounter();
			simd_rsqrt_bench<bx::simd_rsqrt_est>(_dst, _src, _numVertices);
			elapsed += bx::getHPCounter();
		}
		printf("    simd_rsqrt_est: %15f\n", double(elapsed) );
	}

	{
		int64_t elapsed = 0;
		for (uint32_t test = 0; test < numIterations; ++test)
		{
			flushCache();
			elapsed += -bx::getHPCounter();
			simd_rsqrt_bench<bx::simd_rsqrt_nr>(_dst, _src, _numVertices);
			elapsed += bx::getHPCounter();
		}
		printf("     simd_rsqrt_nr: %15f\n", double(elapsed) );
	}

	{
		int64_t elapsed = 0;
		for (uint32_t test = 0; test < numIterations; ++test)
		{
			flushCache();
			elapsed += -bx::getHPCounter();
			simd_rsqrt_bench<bx::simd_rsqrt_carmack>(_dst, _src, _numVertices);
			elapsed += bx::getHPCounter();
		}
		printf("simd_rsqrt_carmack: %15f\n", double(elapsed) );
	}

	{
		int64_t elapsed = 0;
		for (uint32_t test = 0; test < numIterations; ++test)
		{
			flushCache();
			elapsed += -bx::getHPCounter();
			simd_rsqrt_bench<bx::simd_rsqrt>(_dst, _src, _numVertices);
			elapsed += bx::getHPCounter();
		}
		printf("        simd_rsqrt: %15f\n", double(elapsed) );
	}
}

void simd_bench()
{
	bx::DefaultAllocator allocator;
	bx::RngMwc rng;

	const uint32_t numVertices = 1024*1024;

	uint8_t* data = (uint8_t*)BX_ALIGNED_ALLOC(&allocator, 2*numVertices*sizeof(bx::simd128_t), 16);
	bx::simd128_t* src = (bx::simd128_t*)data;
	bx::simd128_t* dst = &src[numVertices];

	printf("\n -- positive & negative --\n");
	for (uint32_t ii = 0; ii < numVertices; ++ii)
	{
		float* ptr = (float*)&src[ii];
		bx::store(ptr, bx::randUnitSphere(&rng) );
		ptr[3] = 1.0f;
	}

	simd_bench_pass(dst, src, numVertices);

	printf("\n -- positive only --\n");
	for (uint32_t ii = 0; ii < numVertices; ++ii)
	{
		float* ptr = (float*)&src[ii];
		ptr[0] = bx::abs(ptr[0]);
		ptr[1] = bx::abs(ptr[1]);
		ptr[2] = bx::abs(ptr[2]);
		ptr[3] = bx::abs(ptr[3]);
	}

	simd_bench_pass(dst, src, numVertices);

	BX_ALIGNED_FREE(&allocator, data, 16);
}


#include <bx/math.h>
#include <bx/timer.h>
#include <bx/file.h>

#include <math.h>

typedef float (*MathFn)(float);

template<MathFn mfn>
float mathTest(const char* _name)
{
	bx::WriterI* writer = bx::getStdOut();
	int64_t elapsed = -bx::getHPCounter();

	float result = 0.0f;
	const float max = 1389.0f;

	for (float xx = 0.0f; xx < max; xx += 0.1f)
	{
		result += mfn(xx);
	}

	bx::Error err;

	elapsed += bx::getHPCounter();
	bx::write(writer, &err, "%-20s: %15f\n", _name, double(elapsed) );

	return result;
}

float rsqrt(float _a)
{
	return 1.0f/::sqrtf(_a);
}

void math_bench()
{
	bx::WriterI* writer = bx::getStdOut();
	bx::Error err;
	bx::write(writer, &err, "Math bench\n\n");

	mathTest<  ::sqrtf    >("  ::sqrtf");
	mathTest<bx::sqrtRef  >("bx::sqrtRef");
	mathTest<bx::sqrtSimd >("bx::sqrtSimd");
	mathTest<bx::sqrt     >("bx::sqrt");

	bx::write(writer, &err, "\n");
	mathTest<  ::rsqrt    >("  ::rsqrtf");
	mathTest<bx::rsqrtRef >("bx::rsqrtRef");
	mathTest<bx::rsqrtSimd>("bx::rsqrtSimd");
	mathTest<bx::rsqrt    >("bx::rsqrt");

	bx::write(writer, &err, "\n");
	mathTest<  ::sinf >("  ::sinf");
	mathTest<bx::sin  >("bx::sin");

	bx::write(writer, &err, "\n");
	mathTest<  ::sinhf>("  ::sinhf");
	mathTest<bx::sinh >("bx::sinh");

	bx::write(writer, &err, "\n");
	mathTest<  ::asinf>("  ::asinf");
	mathTest<bx::asin >("bx::asin");

	bx::write(writer, &err, "\n");
	mathTest<  ::cosf >("  ::cosf");
	mathTest<bx::cos  >("bx::cos");

	bx::write(writer, &err, "\n");
	mathTest<  ::coshf>("  ::coshf");
	mathTest<bx::cosh >("bx::cosh");

	bx::write(writer, &err, "\n");
	mathTest<  ::acosf>("  ::acosf");
	mathTest<bx::acos >("bx::acos");

	bx::write(writer, &err, "\n");
	mathTest<  ::tanf >("  ::tanf");
	mathTest<bx::tan  >("bx::tan");

	bx::write(writer, &err, "\n");
	mathTest<  ::tanhf>("  ::tanhf");
	mathTest<bx::tanh >("bx::tanh");

	bx::write(writer, &err, "\n");
	mathTest<  ::atanf>("  ::atanf");
	mathTest<bx::atan >("bx::atan");

	bx::write(writer, &err, "\n");
	mathTest<  ::expf>("  ::expf");
	mathTest<bx::exp >("bx::exp");

	bx::write(writer, &err, "\n");
	mathTest<  ::exp2f>("  ::exp2f");
	mathTest<bx::exp2 >("bx::exp2");

	bx::write(writer, &err, "\n");
	mathTest<  ::logf >("  ::logf");
	mathTest<bx::log  >("bx::log");

	bx::write(writer, &err, "\n");
	mathTest<  ::log2f>("  ::log2f");
	mathTest<bx::log2 >("bx::log2");
}
