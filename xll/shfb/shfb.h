// shfb.h - Sandcastle Help File Builder
#pragma once

#ifndef SHFB_ORGANIZATION
#define SHFB_ORGANIZATION L"KALX, LLC"
#endif 

#ifndef SHFB_FEEDBACKEMAILADDRESS
#define SHFB_FEEDBACKEMAILADDRESS L"info%40kalx.net"
#endif

#ifdef SHFB_DOCS
#ifdef _WIN64
#define SHFB_DOCS L"..\\..\\docs\\"
#else
#define SHFB_DOCS L"..\\docs\\"
#endif
#endif // SHFB_DOCS