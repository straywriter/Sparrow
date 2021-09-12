#pragma once

#include <function>

namespace Sparrow
{
namespace Util
{
namespace detail
{
struct DefaultTag
{};
} // namespace detail

// This is the wrapper class that most users actually interact with.
// It allows for simple access to registering and instantiating
// singletons.  Create instances of this class in the global scope of
// type Singleton<T> to register your singleton for later access via
// Singleton<T>::try_get().
template <typename T, typename Tag = detail::DefaultTag, typename VaultTag = detail::DefaultTag /* for testing */>
class Singleton
{
public:
  typedef std::function<T *(void)> CreateFunc;
  typedef std::function<void(T *)> TeardownFunc;

  // Generally your program life cycle should be fine with calling
  // get() repeatedly rather than saving the reference, and then not
  // call get() during process shutdown.
  [[deprecated("Replaced by try_get")]] static T *get() { return getEntry().get(); }

  // If, however, you do need to hold a reference to the specific
  // singleton, you can try to do so with a weak_ptr.  Avoid this when
  // possible but the inability to lock the weak pointer can be a
  // signal that the vault has been destroyed.
  [[deprecated("Replaced by try_get")]] static std::weak_ptr<T> get_weak() { return getEntry().get_weak(); }

  // Preferred alternative to get_weak, it returns shared_ptr that can be
  // stored; a singleton won't be destroyed unless shared_ptr is destroyed.
  // Avoid holding these shared_ptrs beyond the scope of a function;
  // don't put them in member variables, always use try_get() instead
  //
  // try_get() can return nullptr if the singleton was destroyed, caller is
  // responsible for handling nullptr return
  static std::shared_ptr<T> try_get() { return getEntry().try_get(); }

  static folly::ReadMostlySharedPtr<T> try_get_fast() { return getEntry().try_get_fast(); }

  /**
   * Applies a callback to the possibly-nullptr singleton instance, returning
   * the callback's result. That is, the following two are functionally
   * equivalent:
   *    singleton.apply(std::ref(f));
   *    f(singleton.try_get().get());
   *
   * For example, the following returns the singleton
   * instance directly without any extra operations on the instance:
   * auto ret = Singleton<T>::apply([](auto* v) { return v; });
   */
  template <typename Func>
  static invoke_result_t<Func, T *> apply(Func f)
  {
    return getEntry().apply(std::ref(f));
  }

  // Quickly ensure the instance exists.
  static void vivify() { getEntry().vivify(); }

  explicit Singleton(std::nullptr_t /* _ */ = nullptr, typename Singleton::TeardownFunc t = nullptr) :
      Singleton([]() { return new T; }, std::move(t))
  {}

  explicit Singleton(typename Singleton::CreateFunc c, typename Singleton::TeardownFunc t = nullptr)
  {
    if (c == nullptr) { detail::singletonThrowNullCreator(typeid(T)); }

    auto vault = SingletonVault::singleton<VaultTag>();
    getEntry().registerSingleton(std::move(c), getTeardownFunc(std::move(t)));
    vault->registerSingleton(&getEntry());
  }

  /**
   * Should be instantiated as soon as "doEagerInit[Via]" is called.
   * Singletons are usually lazy-loaded (built on-demand) but for those which
   * are known to be needed, to avoid the potential lag for objects that take
   * long to construct during runtime, there is an option to make sure these
   * are built up-front.
   *
   * Use like:
   *   Singleton<Foo> gFooInstance = Singleton<Foo>(...).shouldEagerInit();
   *
   * Or alternately, define the singleton as usual, and say
   *   gFooInstance.shouldEagerInit();
   *
   * at some point prior to calling registrationComplete().
   * Then doEagerInit() or doEagerInitVia(Executor*) can be called.
   */
  Singleton &shouldEagerInit()
  {
    auto vault = SingletonVault::singleton<VaultTag>();
    vault->addEagerInitSingleton(&getEntry());
    return *this;
  }

  /**
   * Construct and inject a mock singleton which should be used only from tests.
   * Unlike regular singletons which are initialized once per process lifetime,
   * mock singletons live for the duration of a test. This means that one
   * process running multiple tests can initialize and register the same
   * singleton multiple times. This functionality should be used only from tests
   * since it relaxes validation and performance in order to be able to perform
   * the injection. The returned mock singleton is functionality identical to
   * regular singletons.
   */
  static void make_mock(std::nullptr_t /* c */ = nullptr, typename Singleton<T>::TeardownFunc t = nullptr)
  {
    make_mock([]() { return new T; }, t);
  }

  static void make_mock(CreateFunc c, typename Singleton<T>::TeardownFunc t = nullptr)
  {
    if (c == nullptr) { detail::singletonThrowNullCreator(typeid(T)); }

    auto &entry = getEntry();

    entry.registerSingletonMock(c, getTeardownFunc(t));
  }

private:
  inline static detail::SingletonHolder<T> &getEntry()
  {
    return detail::SingletonHolder<T>::template singleton<Tag, VaultTag>();
  }

  // Construct TeardownFunc.
  static typename detail::SingletonHolder<T>::TeardownFunc getTeardownFunc(TeardownFunc t)
  {
    if (t == nullptr)
    {
      return [](T *v) { delete v; };
    }
    else
    {
      return t;
    }
  }
};

template <typename T, typename Tag = detail::DefaultTag>
class LeakySingleton
{
public:
  using CreateFunc = std::function<T *()>;

  LeakySingleton() : LeakySingleton([] { return new T(); }) {}

  explicit LeakySingleton(CreateFunc createFunc)
  {
    auto &entry = entryInstance();
    if (entry.state != State::NotRegistered) { detail::singletonWarnLeakyDoubleRegistrationAndAbort(entry.type_); }
    entry.createFunc = createFunc;
    entry.state      = State::Dead;
  }

  static T &get() { return instance(); }

  static void make_mock(std::nullptr_t /* c */ = nullptr)
  {
    make_mock([]() { return new T; });
  }

  static void make_mock(CreateFunc createFunc)
  {
    if (createFunc == nullptr) { detail::singletonThrowNullCreator(typeid(T)); }

    auto &entry = entryInstance();
    if (entry.ptr) { annotate_object_leaked(std::exchange(entry.ptr, nullptr)); }
    entry.createFunc = createFunc;
    entry.state      = State::Dead;
  }

private:
  enum class State
  {
    NotRegistered,
    Dead,
    Living
  };

  struct Entry
  {
    Entry() noexcept {}
    Entry(const Entry &) = delete;
    Entry &operator=(const Entry &) = delete;

    std::atomic<State>     state {State::NotRegistered};
    T *                    ptr {nullptr};
    CreateFunc             createFunc;
    std::mutex             mutex;
    detail::TypeDescriptor type_ {typeid(T), typeid(Tag)};
  };

  static Entry &entryInstance() { return detail::createGlobal<Entry, Tag>(); }

  static T &instance()
  {
    auto &entry = entryInstance();
    if (UNLIKELY(entry.state != State::Living)) { createInstance(); }

    return *entry.ptr;
  }

  static void createInstance()
  {
    auto &entry = entryInstance();

    std::lock_guard<std::mutex> lg(entry.mutex);
    if (entry.state == State::Living) { return; }

    if (entry.state == State::NotRegistered)
    {
      detail::singletonWarnLeakyInstantiatingNotRegisteredAndAbort(entry.type_);
    }

    entry.ptr   = entry.createFunc();
    entry.state = State::Living;
  }
};

} // namespace Util

} // namespace Sparrow
