#include <gtest/gtest.h>
#include <stdext/path.h>

TEST(Path, Ctor) {
  {
    stdext::path p;
    EXPECT_STREQ("", p.c_str());
  }
  {
    stdext::path p("a");
    EXPECT_STREQ("a", p.c_str());
  }
  {
    stdext::path p("a/b/c");
    EXPECT_STREQ("a/b/c", p.c_str());
  }
  {
    stdext::path p("/a/b/c");
    EXPECT_STREQ("/a/b/c", p.c_str());
  }
  {
    stdext::path p("a");
    stdext::path q(p);
    EXPECT_STREQ("a", q.c_str());
  }
  {
    stdext::path p("a/b/c");
    stdext::path q(p);
    EXPECT_STREQ("a/b/c", q.c_str());
  }
  {
    stdext::path p("/a/b");
    stdext::path q(p);
    EXPECT_STREQ("/a/b", q.c_str());
  }
}

TEST(Path, WCtor) {
  {
    stdext::wpath p;
    EXPECT_STREQ(L"", p.c_str());
  }
  {
    stdext::wpath p(L"a");
    EXPECT_STREQ(L"a", p.c_str());
  }
  {
    stdext::wpath p(L"a/b/c");
    EXPECT_STREQ(L"a/b/c", p.c_str());
  }
  {
    stdext::wpath p(L"/a/b/c");
    EXPECT_STREQ(L"/a/b/c", p.c_str());
  }
  {
    stdext::wpath p(L"a");
    stdext::wpath q(p);
    EXPECT_STREQ(L"a", q.c_str());
  }
  {
    stdext::wpath p(L"a/b/c");
    stdext::wpath q(p);
    EXPECT_STREQ(L"a/b/c", q.c_str());
  }
  {
    stdext::wpath p(L"/a/b");
    stdext::wpath q(p);
    EXPECT_STREQ(L"/a/b", q.c_str());
  }
}

TEST(Path, Join) {
  {
    stdext::path p;
    p.join("a");
    EXPECT_STREQ("a", p.c_str());
  }
  {
    stdext::path p("a");
    p.join("b");
    p.join("c");

    EXPECT_STREQ("a/b/c", p.c_str());
  }
  {
    stdext::path p("/a");
    p.join("b");
    p.join("c");

    EXPECT_STREQ("/a/b/c", p.c_str());
  }

  {
    stdext::path p;
    stdext::path q("a");
    p.join(q);

    EXPECT_STREQ("a", p.c_str());
  }
  {
    stdext::path p("a");
    stdext::path q("b/c");
    p.join(q);

    EXPECT_STREQ("a/b/c", p.c_str());
  }
  {
    stdext::path p("/a");
    stdext::path q("b/c");
    p.join(q);

    EXPECT_STREQ("/a/b/c", p.c_str());
  }
}

TEST(Path, Parent) {
  {
    stdext::path p;
    EXPECT_STREQ(stdext::path::parentdir, p.parent().c_str());
  }
  {
    stdext::path p("/");
    EXPECT_STREQ("/", p.parent().c_str());
  }
  {
    stdext::path p("a/b/c");
    std::string s;
    EXPECT_STREQ("a/b/c", p.c_str());
    p = p.parent();
    EXPECT_STREQ("a/b", p.c_str());
    p = p.parent();
    EXPECT_STREQ("a", p.c_str());
    p = p.parent();
    EXPECT_STREQ("", p.c_str());
    p = p.parent();
    EXPECT_STREQ(stdext::path::parentdir, p.c_str());
  }
}

TEST(Path, DirAndBase) {
  {
    stdext::path p("/");
    EXPECT_EQ("/", p.dirname());
    EXPECT_EQ("", p.basename());
  }
  {
    stdext::path p("/a");
    EXPECT_EQ("/", p.dirname());
    EXPECT_EQ("a", p.basename());
  }
  {
    stdext::path p("/a/b/c");
    EXPECT_EQ("/a/b", p.dirname());
    EXPECT_EQ("c", p.basename());
  }
}

TEST(Path, RelativeDirAndBase) {
  {
    stdext::path p;
    EXPECT_EQ("", p.dirname());
    EXPECT_EQ("", p.basename());
  }
  {
    stdext::path p("a");
    EXPECT_EQ("", p.dirname());
    EXPECT_EQ("a", p.basename());
  }
  {
    stdext::path p("a/b/c");
    EXPECT_EQ("a/b", p.dirname());
    EXPECT_EQ("c", p.basename());
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
    stdext::path p("a/b/c");
    EXPECT_TRUE(p.relative());
    EXPECT_FALSE(p.root());
  }
  {
    stdext::path p("/");
    EXPECT_FALSE(p.relative());
    EXPECT_TRUE(p.root());
  }
  {
    stdext::path p("/a");
    EXPECT_FALSE(p.relative());
    EXPECT_FALSE(p.root());
  }
  {
    stdext::path p("/a/b/c");
    EXPECT_FALSE(p.relative());
    EXPECT_FALSE(p.root());
  }
}

#if 0
TEST(PathList, Iterator) {
  stdext::path cwd;
  stdext::path::current(cwd);
  std::cout << "Current directory: " << cwd << std::endl;

  stdext::pathList list(cwd);
  for (stdext::path path : list) {
    std::string pathString;
    path.asString(pathString);
    std::cout << pathString << std::endl;
  }
}
#endif

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
