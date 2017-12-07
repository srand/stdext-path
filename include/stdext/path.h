#ifndef STDEXT_PATH_H
#define STDEXT_PATH_H

#include <algorithm>
#include <functional>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>

namespace stdext {

namespace detail {

template<class T>
struct path_helper {};

template<>
struct path_helper<char> {
  static constexpr const char *parentdir = "..";
};

template<>
struct path_helper<wchar_t> {
  static constexpr const wchar_t *parentdir = L"..";
};

}

template <class CharT, class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT>>
class basic_path {

public:
  typedef Traits traits_type;
  typedef CharT value_type;
  typedef Allocator allocator_type;
  typedef typename std::allocator_traits<Allocator>::size_type size_type;
  typedef typename std::allocator_traits<Allocator>::difference_type
      difference_type;
  typedef typename std::allocator_traits<Allocator>::pointer pointer;
  typedef
      typename std::allocator_traits<Allocator>::const_pointer const_pointer;
  typedef std::basic_string<CharT, Traits, Allocator> string_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;

#if defined(_WIN32) || defined(_WIN64)
  static constexpr const CharT separator = '\\';
#else
  static constexpr const CharT separator = '/';
#endif

  static constexpr const CharT *parentdir = detail::path_helper<CharT>::parentdir;

public:
  basic_path() { _rel = true; }

  basic_path(basic_path&& p) noexcept : _comp(std::move(p._comp)), _rel(p._rel), _path(std::move(p._path)) { }

  basic_path(const string_type &p) {
    string_type cp = p;

    if (cp.empty() || cp[0] != separator) {
      _rel = true;
    }

    _trim(cp);

    size_type spos = 0;
    for (size_type epos = cp.find(separator, spos); epos != cp.npos;
         spos = epos + 1, epos = cp.find(separator, spos)) {
      _comp.emplace_back(cp.substr(spos, epos - spos));
    }

    if (!cp.empty()) {
      _comp.emplace_back(cp.substr(spos));
    }
  }

  basic_path(const basic_path<CharT, Traits, Allocator> &p)
      : _comp(p._comp), _rel(p._rel) {}

  basic_path<CharT, Traits, Allocator> & operator= (basic_path<CharT, Traits, Allocator> && p) {
    _comp = std::move(p._comp);
    _rel = p._rel;
    _path = std::move(p._path);
    return *this;
  }

  basic_path<CharT, Traits, Allocator> & operator= (const basic_path<CharT, Traits, Allocator> & p) {
    _comp = p._comp;
    _rel = p._rel;
    _path = p._path;
    return *this;
  }

  string_type str() const {
    std::basic_ostringstream<CharT, Traits, Allocator> os;

    if (_comp.empty()) {
      if (_rel) {
        return std::move(string_type());
      } else {
        return std::move(string_type(1, separator));
      }
    }

    for (auto &comp : _comp) {
      os << separator << comp;
    }

    string_type result = os.str();
    if (_rel) {
      _trim(result);
    }
    return std::move(result);
  }

  const CharT *c_str() const {
    return (_path = str()).c_str();
  }

  bool relative() const { return _rel; }

  bool root() const { return _comp.empty() && !_rel; }

  basic_path<CharT, Traits, Allocator> & join(const string_type &comp) {
    basic_path<CharT, Traits, Allocator> p(comp);
    return join(p);
  }

  basic_path<CharT, Traits, Allocator> & join(const basic_path<CharT, Traits, Allocator> &p) {
    for (auto comp : p._comp) {
      _comp.emplace_back(comp);
    }
    normalize();
    return *this;
  }

  basic_path<CharT, Traits, Allocator> & parent() {
    return join(parentdir);
  }

  basic_path<CharT, Traits, Allocator> parent() const {
    auto p = *this;
    p.join(parentdir);
    return std::move(p);
  }

  basic_path<CharT, Traits, Allocator> & normalize() {
    typename std::vector<string_type>::size_type i = 0;
    while (i < _comp.size()) {
      if (_comp[i] == ".") {
        _comp.erase(_comp.begin() + i);
        continue;
      }
      if (_comp[i] == parentdir && i > 0 && _comp[i - 1] != parentdir) {
        _comp.erase(_comp.begin() + i - 1, _comp.begin() + i + 1);
        i--;
        continue;
      }
      if (_comp[0] == parentdir && !_rel) {
        _comp.erase(_comp.begin());
        continue;
      }
      i++;
    }
    return *this;
  }

  std::tuple<basic_path<CharT, Traits, Allocator>,
             basic_path<CharT, Traits, Allocator>>
  split() const {
    basic_path<CharT, Traits, Allocator> dir(*this);
    basic_path<CharT, Traits, Allocator> base(_comp.empty() ? ""
                                                            : *_comp.rbegin());
    if (!_comp.empty())
      dir.parent();
    return std::make_tuple(std::move(dir), std::move(base));
  }

  string_type dirname() const {
    if (!_comp.empty())
      return parent().str();
    else
      return std::move(str());
  }

  string_type basename() const {
    basic_path<CharT, Traits, Allocator> base(
      _comp.empty() ? "" : *_comp.rbegin());
    return std::move(base.str());
  }

private:
  static int _issep(int c) { return c == separator; }

  static void _ltrim(string_type &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(_issep))));
  }

  static void _rtrim(string_type &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(_issep)))
                .base(),
            s.end());
  }

  static void _trim(string_type &p) {
    _ltrim(p);
    _rtrim(p);
  }

private:
  std::vector<string_type> _comp;
  bool _rel = false;
  mutable string_type _path;
};

typedef basic_path<char> path;
typedef basic_path<wchar_t> wpath;
}

#endif // STDEXT_PATH_H
