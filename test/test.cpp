#include <stdext/path.h>
#include <gtest/gtest.h>

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

TEST(Path, Append) {
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

TEST(Path, Ascend) {
  {
    stdext::path p;
    EXPECT_FALSE(p.ascend());
  }
  {
    stdext::path p("/");
    EXPECT_FALSE(p.ascend());
  }
  {
    stdext::path p("a/b/c");
    std::string s;
    EXPECT_STREQ("a/b/c", p.c_str());
    EXPECT_TRUE(p.ascend());
    EXPECT_STREQ("a/b", p.c_str());
    EXPECT_TRUE(p.ascend());
    EXPECT_STREQ("a", p.c_str());
    EXPECT_TRUE(p.ascend());
    EXPECT_STREQ("", p.c_str());
    EXPECT_FALSE(p.ascend());
    EXPECT_STREQ("", p.c_str());
    EXPECT_FALSE(p.ascend());
  }
}

#if 0
TEST(Path, AbsoluteBaseLast) {
  {
    stdext::path p("/");
    std::string s;

    p.baseComponents(s);
    EXPECT_EQ(std::string("/"), s);
    p.lastComponent(s);
    EXPECT_EQ("", s);
  }
  {
    stdext::path p("/a");
    std::string s;

    p.baseComponents(s);
    EXPECT_EQ("/", s);
    p.lastComponent(s);
    EXPECT_EQ("a", s);
  }
  {
    stdext::path p("/a/b/c");
    std::string s;

    p.baseComponents(s);
    EXPECT_EQ("/a/b", s);
    p.lastComponent(s);
    EXPECT_EQ("c", s);
  }
}

TEST(Path, RelativeBaseLast) {
  {
    stdext::path p;
    std::string s;

    p.baseComponents(s);
    EXPECT_EQ("", s);
    p.lastComponent(s);
    EXPECT_EQ("", s);
  }
  {
    stdext::path p("a");
    std::string s;

    p.baseComponents(s);
    EXPECT_EQ("", s);
    p.lastComponent(s);
    EXPECT_EQ("a", s);
  }
  {
    stdext::path p("a/b/c");
    std::string s;

    p.baseComponents(s);
    EXPECT_EQ("a/b", s);
    p.lastComponent(s);
    EXPECT_EQ("c", s);
  }
}
#endif // if 0

TEST(Path, RelativeOrRoot) {
  {
    stdext::path p;
    EXPECT_TRUE(p.is_relative());
    EXPECT_FALSE(p.is_root());
  }
  {
    stdext::path p("a");
    EXPECT_TRUE(p.is_relative());
    EXPECT_FALSE(p.is_root());
  }
  {
    stdext::path p("a/b/c");
    EXPECT_TRUE(p.is_relative());
    EXPECT_FALSE(p.is_root());
  }
  {
    stdext::path p("/");
    EXPECT_FALSE(p.is_relative());
    EXPECT_TRUE(p.is_root());
  }
  {
    stdext::path p("/a");
    EXPECT_FALSE(p.is_relative());
    EXPECT_FALSE(p.is_root());
  }
  {
    stdext::path p("/a/b/c");
    EXPECT_FALSE(p.is_relative());
    EXPECT_FALSE(p.is_root());
  }
}

#if 0
TEST(Path, Iterator) {
  {
    stdext::path p;
    for (auto &str : p) {
      FAIL();
    }
  }
  {
    stdext::path p("a/b/c");
    auto it = p.begin();
    EXPECT_EQ("a", *it);
    ++it;
    EXPECT_NE(it, p.end());
    EXPECT_EQ("b", *it);
    ++it;
    EXPECT_NE(it, p.end());
    EXPECT_EQ("c", *it);
    ++it;
    EXPECT_EQ(it, p.end());
  }
  {
    stdext::path p("/");
    for (auto &str : p) {
      EXPECT_EQ("/", str);
    }
  }
  {
    stdext::path p("/a/b/c");
    auto it = p.begin();
    EXPECT_EQ("/", *it);
    ++it;
    EXPECT_NE(it, p.end());
    EXPECT_EQ("a", *it);
    ++it;
    EXPECT_NE(it, p.end());
    EXPECT_EQ("b", *it);
    ++it;
    EXPECT_NE(it, p.end());
    EXPECT_EQ("c", *it);
    ++it;
    EXPECT_EQ(it, p.end());
  }
}

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
