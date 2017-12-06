-----------
stdext-path
-----------
Path manipulation utilities in C++

Usage
-----
::
   stdext::path p("a/b/c");
   std::cout << p.str() << endl;                // a/b/c
   std::cout << p.dirname() << endl;            // a/b
   std::cout << p.basename() << endl;           // c
  
   p.join("d");
   std::cout << p.str() << endl;                // a/b/c/d

   p.join("..")
   std::cout << p.str() << endl;                // a/b/c

   auto parent = p.parent();
   std::cout << parent.str() << endl;           // a/b
  
   auto pair = p.split();  
   std::cout << pair.first.str() << endl;       // a/b
   std::cout << pair.second.str() << endl;      // c

   p = stdext::path("a/../b")
   p.normalize();
   std::cout << p.str() << endl;                // b
