make sure you run npm run build again after changing c++ files.

Also when you add a new header file/cpp file :

Add it to binding.gyp
Add it to main.cpp
Do npm rebuild and access it via JS.