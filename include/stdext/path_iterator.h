#ifndef STDEXT_PATH_ITERATOR_H
#define STDEXT_PATH_ITERATOR_H

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#include <dirent.h>
#include <sys/types.h>
#endif

namespace stdext {

namespace detail {
#if defined(_WIN32) || defined(_WIN64)
template <class CharT> struct path_iterator_helper {};

template <> struct path_iterator_helper<char> {
  typedef WIN32_FIND_DATAA data;
  static constexpr const char *match_all = "\\*";
  static HANDLE find(const char *path, data *data) {
    return FindFirstFileA(path, data);
  }
  static bool next(HANDLE h, data *data) { return FindNextFileA(h, data); }
};

template <> struct path_iterator_helper<wchar_t> {
  typedef WIN32_FIND_DATAW data;
  static constexpr const wchar_t *match_all = L"\\*";
  static HANDLE find(const wchar_t *path, data *data) {
    return FindFirstFileW(path, data);
  }
  static bool next(HANDLE h, data *data) { return FindNextFileW(h, data); }
};
#endif
}

template <class CharT, class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT>>
class basic_path_iterator {
  basic_path<CharT, Traits, Allocator> _path;
  basic_path<CharT, Traits, Allocator> _current;

#if defined(_WIN32) || defined(_WIN64)
  typename detail::path_iterator_helper<CharT>::data _data;
  HANDLE _handle;
#else
  DIR *_dir;
  struct dirent _dirent;
  struct dirent *_result;
#endif

public:
  basic_path_iterator() {
#if defined(_WIN32) || defined(_WIN64)
#else
#endif
  }

  basic_path_iterator(const basic_path<CharT, Traits, Allocator> &p)
      : _path(p) {
#if defined(_WIN32) || defined(_WIN64)
    basic_path<CharT, Traits, Allocator> searchpath = _path;
    searchpath.join(detail::path_iterator_helper<CharT>::match_all);
    _handle =
        detail::path_iterator_helper<CharT>::find(searchpath.c_str(), &_data);
#else
    _dir = opendir(p.c_str());
    _result = 0;
#endif
  }

  basic_path_iterator(const basic_path_iterator<CharT, Traits, Allocator> &p)
      : _path(p._path), _current(p._current) {
#if defined(_WIN32) || defined(_WIN64)
    basic_path<CharT, Traits, Allocator> searchpath = _path;
    searchpath.join(detail::path_iterator_helper<CharT>::match_all);
    _handle =
        detail::path_iterator_helper<CharT>::find(searchpath.c_str(), &_data);
#else
    _dir = opendir(p.c_str());
    _result = 0;
#endif
  }

  basic_path_iterator(basic_path_iterator<CharT, Traits, Allocator> &&p)
      : _path(p._path), _current(p._current) {
#if defined(_WIN32) || defined(_WIN64)
    _handle = p._handle;
    p._handle = INVALID_HANDLE_VALUE;
    _data = p._data;
#else
    _dir = p._dir;
    p._dir = 0;
    _result = &_dirent;
    _dirent = p._dirent;
#endif
  }

  ~basic_path_iterator() {
#if defined(_WIN32) || defined(_WIN64)
    if (_handle != INVALID_HANDLE_VALUE) {
      CloseHandle(_handle);
      _handle = INVALID_HANDLE_VALUE;
    }
#else
    if (_dir) {
      closedir(_dir);
      _dir = 0;
    }

#endif
  }

  const basic_path<CharT, Traits, Allocator> &operator*() const {
    return _current;
  }

  const basic_path<CharT, Traits, Allocator> *operator->() const {
    return &_current;
  }

  basic_path_iterator<CharT, Traits, Allocator> &operator++() {
    std::basic_string<CharT, Traits, Allocator> component;
    if (advance(component)) {
      _current = _path;
      _current.join(component);
    } else {
      _path = basic_path<CharT, Traits, Allocator>();
      _current = basic_path<CharT, Traits, Allocator>();
    }
    return *this;
  }

  bool
  operator==(const basic_path_iterator<CharT, Traits, Allocator> &it) const {
    return _path == it._path && _current == it._current;
  }

  bool
  operator!=(const basic_path_iterator<CharT, Traits, Allocator> &it) const {
    return !(*this == it);
  }

private:
  bool advance(std::basic_string<CharT, Traits, Allocator> &component) {
#if defined(_WIN32) || defined(_WIN64)
    while (_handle != INVALID_HANDLE_VALUE) {
      component = _data.cFileName;
      if (!detail::path_iterator_helper<CharT>::next(_handle, &_data)) {
        CloseHandle(_handle);
        _handle = INVALID_HANDLE_VALUE;
      }
      if (component != basic_path<CharT, Traits, Allocator>::currentdir &&
          component != basic_path<CharT, Traits, Allocator>::parentdir) {
        return true;
      }
    }
#else
    if (_dir) {
      int rv = readdir_r(_dir, &_dirent, &_result);
      if (0 == rv && _result != 0) {
        component = _dirent.d_name;
        if (component == basic_path<CharT, Traits, Allocator>::currentdir ||
            component == basic_path<CharT, Traits, Allocator>::parentdir) {
          return advance(component);
        }
        return true;
      }
      closedir(_dir);
      _dir = 0;
    }
#endif
    return false;
  }
};

template <class CharT, class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT>>
basic_path_iterator<CharT, Traits, Allocator>
begin(const basic_path<CharT, Traits, Allocator> &p) {
  return std::move(basic_path_iterator<CharT, Traits, Allocator>(p));
}

template <class CharT, class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT>>
basic_path_iterator<CharT, Traits, Allocator>
end(const basic_path<CharT, Traits, Allocator> &p) {
  return std::move(basic_path_iterator<CharT, Traits, Allocator>());
}

} // stdext

#endif // STDEXT_PATH_ITERATOR_H
