#include <gtest/gtest.h>
#include <stdext/path.h>
#include <stdext/path_iterator.h>
#include <memory>

#if defined(_WIN32) || defined(_WIN64)
std::string to_winsep(const char *s) {
  std::string t(s);
  std::replace(t.begin(), t.end(), '/', '\\');
  return t;
}
std::wstring to_winsep(const wchar_t *s) {
  std::wstring t(s);
  std::replace(t.begin(), t.end(), L'/', L'\\');
  return t;
}
#define P(x) to_winsep(x).c_str()
#else
#define P(x) x
#endif


TEST(Path, Ctor) {
  {
    stdext::path p;
    EXPECT_STREQ(P(""), p.c_str());
  }
  {
    stdext::path p(P("a"));
    EXPECT_STREQ(P("a"), p.c_str());
  }
  {
    stdext::path p(P("a/b/c"));
    EXPECT_STREQ(P("a/b/c"), p.c_str());
  }
  {
    stdext::path p(P("/a/b/c"));
    EXPECT_STREQ(P("/a/b/c"), p.c_str());
  }
  {
    stdext::path p(P("a"));
    stdext::path q(p);
    EXPECT_STREQ(P("a"), q.c_str());
  }
  {
    stdext::path p(P("a/b/c"));
    stdext::path q(p);
    EXPECT_STREQ(P("a/b/c"), q.c_str());
  }
  {
    stdext::path p(P("/a/b"));
    stdext::path q(p);
    EXPECT_STREQ(P("/a/b"), q.c_str());
  }
}

TEST(Path, WCtor) {
  {
    stdext::wpath p;
    EXPECT_STREQ(P(L""), p.c_str());
  }
  {
    stdext::wpath p(P(L"a"));
    EXPECT_STREQ(P(L"a"), p.c_str());
  }
  {
    stdext::wpath p(P(L"a/b/c"));
    EXPECT_STREQ(P(L"a/b/c"), p.c_str());
  }
  {
    stdext::wpath p(P(L"/a/b/c"));
    EXPECT_STREQ(P(L"/a/b/c"), p.c_str());
  }
  {
    stdext::wpath p(P(L"a"));
    stdext::wpath q(p);
    EXPECT_STREQ(P(L"a"), q.c_str());
  }
  {
    stdext::wpath p(P(L"a/b/c"));
    stdext::wpath q(p);
    EXPECT_STREQ(P(L"a/b/c"), q.c_str());
  }
  {
    stdext::wpath p(P(L"/a/b"));
    stdext::wpath q(p);
    EXPECT_STREQ(P(L"/a/b"), q.c_str());
  }
}

TEST(Path, Join) {
  {
    stdext::path p;
    EXPECT_STREQ("a", p.join(P("a")).c_str());
  }
  {
    stdext::path p("a");
    p.join("b");
    p.join("c");

    EXPECT_STREQ(P("a/b/c"), p.c_str());
  }
  {
    stdext::path p(P("/a"));
    p.join("b");
    p.join("c");

    EXPECT_STREQ(P("/a/b/c"), p.c_str());
  }

  {
    stdext::path p;
    stdext::path q("a");
    p.join(q);

    EXPECT_STREQ("a", p.c_str());
  }
  {
    stdext::path p("a");
    stdext::path q(P("b/c"));
    p.join(q);

    EXPECT_STREQ(P("a/b/c"), p.c_str());
  }
  {
    stdext::path p(P("/a"));
    stdext::path q(P("b/c"));
    p.join(q);

    EXPECT_STREQ(P("/a/b/c"), p.c_str());
  }
}

TEST(Path, Parent) {
  {
    stdext::path p;
    EXPECT_STREQ(stdext::path::parentdir, p.parent().c_str());
  }
  {
    stdext::path p(P("/"));
    EXPECT_STREQ(P("/"), p.parent().c_str());
  }
  {
    stdext::path p(P("a/b/c"));
    EXPECT_STREQ(P("a/b/c"), p.c_str());
    EXPECT_STREQ(P("a/b"), p.parent().c_str());
    EXPECT_STREQ(P("a"), p.parent().c_str());
    EXPECT_STREQ(P(""), p.parent().c_str());
    EXPECT_STREQ(stdext::path::parentdir, p.parent().c_str());
  }
}

TEST(Path, DirAndBase) {
  stdext::path dir, base;
  {
    stdext::path p(P("/"));
    EXPECT_EQ(P("/"), p.dirname());
    EXPECT_EQ(P(""), p.basename());
    std::tie(dir, base) = p.split();
    EXPECT_EQ(P("/"), dir.str());
    EXPECT_EQ(P(""), base.str());
  }
  {
    stdext::path p(P("/a"));
    EXPECT_EQ(P("/"), p.dirname());
    EXPECT_EQ(P("a"), p.basename());
    std::tie(dir, base) = p.split();
    EXPECT_EQ(P("/"), dir.str());
    EXPECT_EQ(P("a"), base.str());
  }
  {
    stdext::path p(P("/a/b/c"));
    EXPECT_EQ(P("/a/b"), p.dirname());
    EXPECT_EQ(P("c"), p.basename());
    std::tie(dir, base) = p.split();
    EXPECT_EQ(P("/a/b"), dir.str());
    EXPECT_EQ(P("c"), base.str());
  }
}

TEST(Path, RelativeDirAndBase) {
  stdext::path dir, base;
  {
    stdext::path p;
    EXPECT_EQ("", p.dirname());
    EXPECT_EQ("", p.basename());
    std::tie(dir, base) = p.split();
    EXPECT_EQ("", dir.str());
    EXPECT_EQ("", base.str());
  }
  {
    stdext::path p("a");
    EXPECT_EQ("", p.dirname());
    EXPECT_EQ("a", p.basename());
    std::tie(dir, base) = p.split();
    EXPECT_EQ("", dir.str());
    EXPECT_EQ("a", base.str());
  }
  {
    stdext::path p(P("a/b/c"));
    EXPECT_EQ(P("a/b"), p.dirname());
    EXPECT_EQ(P("c"), p.basename());
    std::tie(dir, base) = p.split();
    EXPECT_EQ(P("a/b"), dir.str());
    EXPECT_EQ(P("c"), base.str());
  }
}

TEST(Path, RelativeOrRoot) {
  {
    stdext::path p;
    EXPECT_TRUE(p.relative());
    EXPECT_FALSE(p.root());
  }
  {
    stdext::path p("a");
    EXPECT_TRUE(p.relative());
    EXPECT_FALSE(p.root());
  }
  {
    stdext::path p(P("a/b/c"));
    EXPECT_TRUE(p.relative());
    EXPECT_FALSE(p.root());
  }
  {
    stdext::path p(P("/"));
    EXPECT_FALSE(p.relative());
    EXPECT_TRUE(p.root());
  }
  {
    stdext::path p(P("/a"));
    EXPECT_FALSE(p.relative());
    EXPECT_FALSE(p.root());
  }
  {
    stdext::path p(P("/a/b/c"));
    EXPECT_FALSE(p.relative());
    EXPECT_FALSE(p.root());
  }
}

TEST(Path, Iterator) {
  using namespace stdext;

  stdext::path cwd(".");
  for (stdext::path path : cwd) {
    std::cout << path.str() << std::endl;
  }
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
