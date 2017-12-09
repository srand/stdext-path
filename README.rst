===========
stdext-path
===========
Path manipulation utilities in C++

Usage
-----

Example:
::
   stdext::path p("a/b/c");
   std::cout << p.str() << std::endl;                // a/b/c
   std::cout << p.dirname() << std::endl;            // a/b
   std::cout << p.basename() << std::endl;           // c
  
   std::cout << p.join("d").str() << std::endl;      // a/b/c/d
   std::cout << p.join("..").str() << std::endl;     // a/b/c
   std::cout << p.parent().str() << std::endl;       // a/b
  
   stdext::path dir, base;
   std::tie(dir, base) = p.split();
   std::cout << dir.str() << std::endl;              // a
   std::cout << base.str() << std::endl;             // b

   p = stdext::path("a/../b")
   std::cout << p.normalize().str() << std::endl;    // b

   for (auto path : stdext::path(".")) {             // enumerate current directory
       std::cout << path.str() << std::endl;
   }

Build
-----

Use Pam, http://github.com/srand/pam
::
   from externals.stdext import stdext_path

   cxx_executable(
      name = "hello",
      sources = ["hello.cpp"],
      dependencies = [stdext_path]
   )
