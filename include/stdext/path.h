// Public domain

#ifndef STDEXT_PATH_H
#define STDEXT_PATH_H

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace stdext {

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

  static constexpr const CharT separator = '/';
  static constexpr const CharT *parentdir = "..";

public:
  basic_path() { _rel = true; }

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

  string_type str() const {
    std::basic_ostringstream<CharT, Traits, Allocator> os;

    if (_comp.empty()) {
      if (_rel) {
        _path.clear();
      } else {
        _path = separator;
      }
      return _path.c_str();
    }

    for (auto &comp : _comp) {
      os << separator << comp;
    }

    _path = os.str();
    if (_rel) {
      _trim(_path);
    }
    return _path;
  }

  const CharT *c_str() const {
    str();
    return _path.c_str();
  }

  bool relative() const { return _rel; }

  bool root() const { return _comp.empty() && !_rel; }

  bool is_directory() const {
    struct stat st;
    if (0 != stat(c_str(), &st)) {
      return false;
    }
    return S_ISDIR(st.st_mode);
  }

  void join(const string_type &comp) {
    basic_path<CharT, Traits, Allocator> p(comp);
    join(p);
  }

  void join(const basic_path<CharT, Traits, Allocator> &p) {
    for (auto comp : p._comp) {
      _comp.emplace_back(comp);
    }
    normalize();
  }

  basic_path<CharT, Traits, Allocator> parent() const {
    auto p = *this;
    p.join(parentdir);
    return p;
  }

  void normalize() {
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
  }

  std::pair<basic_path<CharT, Traits, Allocator>,
            basic_path<CharT, Traits, Allocator>>
  split() const {
    basic_path<CharT, Traits, Allocator> dir(*this);
    basic_path<CharT, Traits, Allocator> base(_comp.empty() ? ""
                                                            : *_comp.rbegin());
    if (!_comp.empty())
      dir.join(parentdir);
    return std::pair<basic_path<CharT, Traits, Allocator>,
                     basic_path<CharT, Traits, Allocator>>(dir, base);
  }

  string_type dirname() const {
    auto pair = split();
    return pair.first.str();
  }

  string_type basename() const {
    auto pair = split();
    return pair.second.str();
  }

private:
  static int _issep(int c) { return c == '/' || c == '\\'; }

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
