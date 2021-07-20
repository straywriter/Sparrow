#pragma once
/*
 * Copyright 2013 Jeremie Roy. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

/// Inspired from texture-atlas from freetype-gl (http://code.google.com/p/freetype-gl/)
/// by Nicolas Rougier (Nicolas.Rougier@inria.fr)
/// The actual implementation is based on the article by Jukka Jylänki : "A
/// Thousand Ways to Pack the Bin - A Practical Approach to Two-Dimensional
/// Rectangle Bin Packing", February 27, 2010.
/// More precisely, this is an implementation of the Skyline Bottom-Left
/// algorithm based on C++ sources provided by Jukka Jylänki at:
/// http://clb.demon.fi/files/RectangleBinPack/

#include <Render/Render.h>


#include <limits.h> // INT_MAX
#include <vector>


#include <bx/timer.h>
#include <bx/math.h>


struct AtlasRegion
{
    enum Type
    {
        TYPE_GRAY = 1, // 1 component
        TYPE_BGRA8 = 4 // 4 components
    };

    uint16_t x, y;
    uint16_t width, height;
    uint32_t mask; // encode the region type, the face index and the component index in case of a gray region

    Type getType() const
    {
        return (Type)((mask >> 0) & 0x0000000F);
    }

    uint32_t getFaceIndex() const
    {
        return (mask >> 4) & 0x0000000F;
    }

    uint32_t getComponentIndex() const
    {
        return (mask >> 8) & 0x0000000F;
    }

    void setMask(Type _type, uint32_t _faceIndex, uint32_t _componentIndex)
    {
        mask = (_componentIndex << 8) + (_faceIndex << 4) + (uint32_t)_type;
    }
};

class Atlas
{
  public:
    /// create an empty dynamic atlas (region can be updated and added)
    /// @param textureSize an atlas creates a texture cube of 6 faces with size equal to (textureSize*textureSize *
    /// sizeof(RGBA) )
    /// @param maxRegionCount maximum number of region allowed in the atlas
    Atlas(uint16_t _textureSize, uint16_t _maxRegionsCount = 4096);

    /// initialize a static atlas with serialized data	(region can be updated but not added)
    /// @param textureSize an atlas creates a texture cube of 6 faces with size equal to (textureSize*textureSize *
    /// sizeof(RGBA) )
    /// @param textureBuffer buffer of size 6*textureSize*textureSize*sizeof(uint32_t) (will be copied)
    /// @param regionCount number of region in the Atlas
    /// @param regionBuffer buffer containing the region (will be copied)
    /// @param maxRegionCount maximum number of region allowed in the atlas
    Atlas(uint16_t _textureSize, const uint8_t *_textureBuffer, uint16_t _regionCount, const uint8_t *_regionBuffer,
          uint16_t _maxRegionsCount = 4096);
    ~Atlas();

    /// add a region to the atlas, and copy the content of mem to the underlying texture
    uint16_t addRegion(uint16_t _width, uint16_t _height, const uint8_t *_bitmapBuffer,
                       AtlasRegion::Type _type = AtlasRegion::TYPE_BGRA8, uint16_t outline = 0);

    /// update a preallocated region
    void updateRegion(const AtlasRegion &_region, const uint8_t *_bitmapBuffer);

    /// Pack the UV coordinates of the four corners of a region to a vertex buffer using the supplied vertex format.
    /// v0 -- v3
    /// |     |     encoded in that order:  v0,v1,v2,v3
    /// v1 -- v2
    /// @remark the UV are four signed short normalized components.
    /// @remark the x,y,z components encode cube uv coordinates. The w component encode the color channel if any.
    /// @param handle handle to the region we are interested in
    /// @param vertexBuffer address of the first vertex we want to update. Must be valid up to vertexBuffer + offset +
    /// 3*stride + 4*sizeof(int16_t), which means the buffer must contains at least 4 vertex includind the first.
    /// @param offset byte offset to the first uv coordinate of the vertex in the buffer
    /// @param stride stride between tho UV coordinates, usually size of a Vertex.
    void packUV(uint16_t _regionHandle, uint8_t *_vertexBuffer, uint32_t _offset, uint32_t _stride) const;
    void packUV(const AtlasRegion &_region, uint8_t *_vertexBuffer, uint32_t _offset, uint32_t _stride) const;

    /// Same as packUV but pack a whole face of the atlas cube, mostly used for debugging and visualizing atlas
    void packFaceLayerUV(uint32_t _idx, uint8_t *_vertexBuffer, uint32_t _offset, uint32_t _stride) const;

    /// return the TextureHandle (cube) of the atlas
    bgfx::TextureHandle getTextureHandle() const
    {
        return m_textureHandle;
    }

    // retrieve a region info
    const AtlasRegion &getRegion(uint16_t _handle) const
    {
        return m_regions[_handle];
    }

    /// retrieve the size of side of a texture in pixels
    uint16_t getTextureSize() const
    {
        return m_textureSize;
    }

    /// retrieve the usage ratio of the atlas
    // float getUsageRatio() const { return 0.0f; }

    /// retrieve the numbers of region in the atlas
    uint16_t getRegionCount() const
    {
        return m_regionCount;
    }

    /// retrieve a pointer to the region buffer (in order to serialize it)
    const AtlasRegion *getRegionBuffer() const
    {
        return m_regions;
    }

    /// retrieve the byte size of the texture
    uint32_t getTextureBufferSize() const
    {
        return 6 * m_textureSize * m_textureSize * 4;
    }

    /// retrieve the mirrored texture buffer (to serialize it)
    const uint8_t *getTextureBuffer() const
    {
        return m_textureBuffer;
    }

  private:
    void init();

    struct PackedLayer;
    PackedLayer *m_layers;
    AtlasRegion *m_regions;
    uint8_t *m_textureBuffer;

    uint32_t m_usedLayers;
    uint32_t m_usedFaces;

    bgfx::TextureHandle m_textureHandle;
    uint16_t m_textureSize;
    float m_texelSize;
    float m_texelOffset[2];

    uint16_t m_regionCount;
    uint16_t m_maxRegionCount;
};



class RectanglePacker
{
public:
	RectanglePacker();
	RectanglePacker(uint32_t _width, uint32_t _height);

	/// non constructor initialization
	void init(uint32_t _width, uint32_t _height);

	/// find a suitable position for the given rectangle
	/// @return true if the rectangle can be added, false otherwise
	bool addRectangle(uint16_t _width, uint16_t _height, uint16_t& _outX, uint16_t& _outY);

	/// return the used surface in squared unit
	uint32_t getUsedSurface()
	{
		return m_usedSpace;
	}

	/// return the total available surface in squared unit
	uint32_t getTotalSurface()
	{
		return m_width * m_height;
	}

	/// return the usage ratio of the available surface [0:1]
	float getUsageRatio();

	/// reset to initial state
	void clear();

private:
	int32_t fit(uint32_t _skylineNodeIndex, uint16_t _width, uint16_t _height);

	/// Merges all skyline nodes that are at the same level.
	void merge();

	struct Node
	{
		Node(int16_t _x, int16_t _y, int16_t _width) : x(_x), y(_y), width(_width)
		{
		}

		int16_t x;     //< The starting x-coordinate (leftmost).
		int16_t y;     //< The y-coordinate of the skyline level line.
		int32_t width; //< The line _width. The ending coordinate (inclusive) will be x+width-1.
	};


	uint32_t m_width;            //< width (in pixels) of the underlying texture
	uint32_t m_height;           //< height (in pixels) of the underlying texture
	uint32_t m_usedSpace;        //< Surface used in squared pixel
	std::vector<Node> m_skyline; //< node of the skyline algorithm
};

RectanglePacker::RectanglePacker()
	: m_width(0)
	, m_height(0)
	, m_usedSpace(0)
{
}

RectanglePacker::RectanglePacker(uint32_t _width, uint32_t _height)
	: m_width(_width)
	, m_height(_height)
	, m_usedSpace(0)
{
	// We want a one pixel border around the whole atlas to avoid any artefact when
	// sampling texture
	m_skyline.push_back(Node(1, 1, uint16_t(_width - 2) ) );
}

void RectanglePacker::init(uint32_t _width, uint32_t _height)
{
	BX_ASSERT(_width > 2, "_width must be > 2");
	BX_ASSERT(_height > 2, "_height must be > 2");
	m_width = _width;
	m_height = _height;
	m_usedSpace = 0;

	m_skyline.clear();
	// We want a one pixel border around the whole atlas to avoid any artifact when
	// sampling texture
	m_skyline.push_back(Node(1, 1, uint16_t(_width - 2) ) );
}

bool RectanglePacker::addRectangle(uint16_t _width, uint16_t _height, uint16_t& _outX, uint16_t& _outY)
{
	int best_height, best_index;
	int32_t best_width;
	Node* node;
	Node* prev;
	_outX = 0;
	_outY = 0;

	best_height = INT_MAX;
	best_index = -1;
	best_width = INT_MAX;
	for (uint16_t ii = 0, num = uint16_t(m_skyline.size() ); ii < num; ++ii)
	{
		int32_t yy = fit(ii, _width, _height);
		if (yy >= 0)
		{
			node = &m_skyline[ii];
			if ( ( (yy + _height) < best_height)
			|| ( ( (yy + _height) == best_height) && (node->width < best_width) ) )
			{
				best_height = uint16_t(yy) + _height;
				best_index = ii;
				best_width = node->width;
				_outX = node->x;
				_outY = uint16_t(yy);
			}
		}
	}

	if (best_index == -1)
	{
		return false;
	}

	Node newNode(_outX, _outY + _height, _width);
	m_skyline.insert(m_skyline.begin() + best_index, newNode);

	for (uint16_t ii = uint16_t(best_index + 1), num = uint16_t(m_skyline.size() ); ii < num; ++ii)
	{
		node = &m_skyline[ii];
		prev = &m_skyline[ii - 1];
		if (node->x < (prev->x + prev->width) )
		{
			uint16_t shrink = uint16_t(prev->x + prev->width - node->x);
			node->x += shrink;
			node->width -= shrink;
			if (node->width <= 0)
			{
				m_skyline.erase(m_skyline.begin() + ii);
				--ii;
				--num;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	merge();
	m_usedSpace += _width * _height;
	return true;
}

float RectanglePacker::getUsageRatio()
{
	uint32_t total = m_width * m_height;
	if (total > 0)
	{
		return (float)m_usedSpace / (float)total;
	}

	return 0.0f;
}

void RectanglePacker::clear()
{
	m_skyline.clear();
	m_usedSpace = 0;

	// We want a one pixel border around the whole atlas to avoid any artefact when
	// sampling texture
	m_skyline.push_back(Node(1, 1, uint16_t(m_width - 2) ) );
}

int32_t RectanglePacker::fit(uint32_t _skylineNodeIndex, uint16_t _width, uint16_t _height)
{
	int32_t width = _width;
	int32_t height = _height;

	const Node& baseNode = m_skyline[_skylineNodeIndex];

	int32_t xx = baseNode.x, yy;
	int32_t widthLeft = width;
	int32_t ii = _skylineNodeIndex;

	if ( (xx + width) > (int32_t)(m_width - 1) )
	{
		return -1;
	}

	yy = baseNode.y;
	while (widthLeft > 0)
	{
		const Node& node = m_skyline[ii];
		if (node.y > yy)
		{
			yy = node.y;
		}

		if ( (yy + height) > (int32_t)(m_height - 1) )
		{
			return -1;
		}

		widthLeft -= node.width;
		++ii;
	}

	return yy;
}

void RectanglePacker::merge()
{
	Node* node;
	Node* next;
	uint32_t ii;

	for (ii = 0; ii < m_skyline.size() - 1; ++ii)
	{
		node = (Node*) &m_skyline[ii];
		next = (Node*) &m_skyline[ii + 1];
		if (node->y == next->y)
		{
			node->width += next->width;
			m_skyline.erase(m_skyline.begin() + ii + 1);
			--ii;
		}
	}
}

struct Atlas::PackedLayer
{
	RectanglePacker packer;
	AtlasRegion faceRegion;
};

Atlas::Atlas(uint16_t _textureSize, uint16_t _maxRegionsCount)
	: m_usedLayers(0)
	, m_usedFaces(0)
	, m_textureSize(_textureSize)
	, m_regionCount(0)
	, m_maxRegionCount(_maxRegionsCount)
{
	BX_ASSERT(_textureSize >= 64 && _textureSize <= 4096, "Invalid _textureSize %d.", _textureSize);
	BX_ASSERT(_maxRegionsCount >= 64 && _maxRegionsCount <= 32000, "Invalid _maxRegionsCount %d.", _maxRegionsCount);

	init();

	m_layers = new PackedLayer[24];
	for (int ii = 0; ii < 24; ++ii)
	{
		m_layers[ii].packer.init(_textureSize, _textureSize);
	}

	m_regions = new AtlasRegion[_maxRegionsCount];
	m_textureBuffer = new uint8_t[ _textureSize * _textureSize * 6 * 4 ];
	bx::memSet(m_textureBuffer, 0, _textureSize * _textureSize * 6 * 4);

	m_textureHandle = bgfx::createTextureCube(_textureSize
		, false
		, 1
		, bgfx::TextureFormat::BGRA8
		);
}

Atlas::Atlas(uint16_t _textureSize, const uint8_t* _textureBuffer, uint16_t _regionCount, const uint8_t* _regionBuffer, uint16_t _maxRegionsCount)
	: m_usedLayers(24)
	, m_usedFaces(6)
	, m_textureSize(_textureSize)
	, m_regionCount(_regionCount)
	, m_maxRegionCount(_regionCount < _maxRegionsCount ? _regionCount : _maxRegionsCount)
{
	BX_ASSERT(_regionCount <= 64 && _maxRegionsCount <= 4096, "_regionCount %d, _maxRegionsCount %d", _regionCount, _maxRegionsCount);

	init();

	m_regions = new AtlasRegion[_regionCount];
	m_textureBuffer = new uint8_t[getTextureBufferSize()];

	bx::memCopy(m_regions, _regionBuffer, _regionCount * sizeof(AtlasRegion) );
	bx::memCopy(m_textureBuffer, _textureBuffer, getTextureBufferSize() );

	m_textureHandle = bgfx::createTextureCube(_textureSize
		, false
		, 1
		, bgfx::TextureFormat::BGRA8
		, BGFX_SAMPLER_NONE
		, bgfx::makeRef(m_textureBuffer, getTextureBufferSize() )
		);
}

Atlas::~Atlas()
{
	bgfx::destroy(m_textureHandle);

	delete [] m_layers;
	delete [] m_regions;
	delete [] m_textureBuffer;
}

void Atlas::init()
{
	m_texelSize = float(UINT16_MAX) / float(m_textureSize);
	float texelHalf = m_texelSize/2.0f;
	switch (bgfx::getRendererType() )
	{
	case bgfx::RendererType::Direct3D9:
		m_texelOffset[0] = 0.0f;
		m_texelOffset[1] = 0.0f;
		break;

	case bgfx::RendererType::Direct3D11:
	case bgfx::RendererType::Direct3D12:
		m_texelOffset[0] = texelHalf;
		m_texelOffset[1] = texelHalf;
		break;

	default:
		m_texelOffset[0] = texelHalf;
		m_texelOffset[1] = -texelHalf;
		break;
	}
}

uint16_t Atlas::addRegion(uint16_t _width, uint16_t _height, const uint8_t* _bitmapBuffer, AtlasRegion::Type _type, uint16_t outline)
{
	if (m_regionCount >= m_maxRegionCount)
	{
		return UINT16_MAX;
	}

	uint16_t xx = 0;
	uint16_t yy = 0;
	uint32_t idx = 0;
	while (idx < m_usedLayers)
	{
		if (m_layers[idx].faceRegion.getType() == _type
		&&  m_layers[idx].packer.addRectangle(_width + 1, _height + 1, xx, yy) )
		{
			break;
		}

		idx++;
	}

	if (idx >= m_usedLayers)
	{
		if ( (idx + _type) > 24
		|| m_usedFaces >= 6)
		{
			return UINT16_MAX;
		}

		for (int ii = 0; ii < _type; ++ii)
		{
			AtlasRegion& region = m_layers[idx + ii].faceRegion;
			region.x = 0;
			region.y = 0;
			region.width = m_textureSize;
			region.height = m_textureSize;
			region.setMask(_type, m_usedFaces, ii);
		}

		m_usedLayers += _type;
		m_usedFaces++;

		if (!m_layers[idx].packer.addRectangle(_width + 1, _height + 1, xx, yy) )
		{
			return UINT16_MAX;
		}
	}

	AtlasRegion& region = m_regions[m_regionCount];
	region.x = xx;
	region.y = yy;
	region.width = _width;
	region.height = _height;
	region.mask = m_layers[idx].faceRegion.mask;

	updateRegion(region, _bitmapBuffer);

	region.x += outline;
	region.y += outline;
	region.width -= (outline * 2);
	region.height -= (outline * 2);

	return m_regionCount++;
}

void Atlas::updateRegion(const AtlasRegion& _region, const uint8_t* _bitmapBuffer)
{
	uint32_t size = _region.width * _region.height * 4;
	if (0 < size)
	{
		const bgfx::Memory* mem = bgfx::alloc(size);
		bx::memSet(mem->data, 0, mem->size);
		if (_region.getType() == AtlasRegion::TYPE_BGRA8)
		{
			const uint8_t* inLineBuffer = _bitmapBuffer;
			uint8_t* outLineBuffer = m_textureBuffer + _region.getFaceIndex() * (m_textureSize * m_textureSize * 4) + ( ( (_region.y * m_textureSize) + _region.x) * 4);

			for (int yy = 0; yy < _region.height; ++yy)
			{
				bx::memCopy(outLineBuffer, inLineBuffer, _region.width * 4);
				inLineBuffer += _region.width * 4;
				outLineBuffer += m_textureSize * 4;
			}

			bx::memCopy(mem->data, _bitmapBuffer, mem->size);
		}
		else
		{
			uint32_t layer = _region.getComponentIndex();
			const uint8_t* inLineBuffer = _bitmapBuffer;
			uint8_t* outLineBuffer = (m_textureBuffer + _region.getFaceIndex() * (m_textureSize * m_textureSize * 4) + ( ( (_region.y * m_textureSize) + _region.x) * 4) );

			for (int yy = 0; yy < _region.height; ++yy)
			{
				for (int xx = 0; xx < _region.width; ++xx)
				{
					outLineBuffer[(xx * 4) + layer] = inLineBuffer[xx];
				}

				bx::memCopy(mem->data + yy * _region.width * 4, outLineBuffer, _region.width * 4);
				inLineBuffer += _region.width;
				outLineBuffer += m_textureSize * 4;
			}
		}

		bgfx::updateTextureCube(m_textureHandle, 0, (uint8_t)_region.getFaceIndex(), 0, _region.x, _region.y, _region.width, _region.height, mem);
	}
}

void Atlas::packFaceLayerUV(uint32_t _idx, uint8_t* _vertexBuffer, uint32_t _offset, uint32_t _stride) const
{
	packUV(m_layers[_idx].faceRegion, _vertexBuffer, _offset, _stride);
}

void Atlas::packUV(uint16_t _regionHandle, uint8_t* _vertexBuffer, uint32_t _offset, uint32_t _stride) const
{
	const AtlasRegion& region = m_regions[_regionHandle];
	packUV(region, _vertexBuffer, _offset, _stride);
}

static void writeUV(uint8_t* _vertexBuffer, int16_t _x, int16_t _y, int16_t _z, int16_t _w)
{
	uint16_t* xyzw = (uint16_t*)_vertexBuffer;
	xyzw[0] = _x;
	xyzw[1] = _y;
	xyzw[2] = _z;
	xyzw[3] = _w;
}

void Atlas::packUV(const AtlasRegion& _region, uint8_t* _vertexBuffer, uint32_t _offset, uint32_t _stride) const
{
	int16_t x0 = (int16_t)( ( (float)_region.x * m_texelSize + m_texelOffset[0]) - float(INT16_MAX) );
	int16_t y0 = (int16_t)( ( (float)_region.y * m_texelSize + m_texelOffset[1]) - float(INT16_MAX) );
	int16_t x1 = (int16_t)( ( ( (float)_region.x + _region.width) * m_texelSize + m_texelOffset[0]) - float(INT16_MAX) );
	int16_t y1 = (int16_t)( ( ( (float)_region.y + _region.height) * m_texelSize + m_texelOffset[1]) - float(INT16_MAX) );
	int16_t ww = (int16_t)( (float(INT16_MAX) / 4.0f) * (float)_region.getComponentIndex() );

	_vertexBuffer += _offset;
	switch (_region.getFaceIndex() )
	{
	case 0: // +X
		x0 = -x0;
		x1 = -x1;
		y0 = -y0;
		y1 = -y1;
		writeUV(_vertexBuffer, INT16_MAX, y0, x0, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, INT16_MAX, y1, x0, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, INT16_MAX, y1, x1, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, INT16_MAX, y0, x1, ww); _vertexBuffer += _stride;
		break;

	case 1: // -X
		y0 = -y0;
		y1 = -y1;
		writeUV(_vertexBuffer, INT16_MIN, y0, x0, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, INT16_MIN, y1, x0, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, INT16_MIN, y1, x1, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, INT16_MIN, y0, x1, ww); _vertexBuffer += _stride;
		break;

	case 2: // +Y
		writeUV(_vertexBuffer, x0, INT16_MAX, y0, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x0, INT16_MAX, y1, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x1, INT16_MAX, y1, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x1, INT16_MAX, y0, ww); _vertexBuffer += _stride;
		break;

	case 3: // -Y
		y0 = -y0;
		y1 = -y1;
		writeUV(_vertexBuffer, x0, INT16_MIN, y0, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x0, INT16_MIN, y1, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x1, INT16_MIN, y1, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x1, INT16_MIN, y0, ww); _vertexBuffer += _stride;
		break;

	case 4: // +Z
		y0 = -y0;
		y1 = -y1;
		writeUV(_vertexBuffer, x0, y0, INT16_MAX, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x0, y1, INT16_MAX, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x1, y1, INT16_MAX, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x1, y0, INT16_MAX, ww); _vertexBuffer += _stride;
		break;

	case 5: // -Z
		x0 = -x0;
		x1 = -x1;
		y0 = -y0;
		y1 = -y1;
		writeUV(_vertexBuffer, x0, y0, INT16_MIN, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x0, y1, INT16_MIN, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x1, y1, INT16_MIN, ww); _vertexBuffer += _stride;
		writeUV(_vertexBuffer, x1, y0, INT16_MIN, ww); _vertexBuffer += _stride;
		break;
	}
}
