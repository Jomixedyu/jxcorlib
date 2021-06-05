#ifndef CORELIB_DEBUGTOOL
#define CORELIB_DEBUGTOOL

//C++20
#define DEBUG_INFO(info) std::format("info: {}; line: {}, file: {};", info, __LINE__, __FILE__);

#endif // !CORELIB_DEBUGTOOL
