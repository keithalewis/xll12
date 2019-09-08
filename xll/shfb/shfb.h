// shfb.h - Sandcastle Help File Builder
#pragma once

#ifndef SHFB_ORGANIZATION
#define SHFB_ORGANIZATION L"KALX, LLC"
#endif 

#ifndef SHFB_FEEDBACKEMAILADDRESS
#define SHFB_FEEDBACKEMAILADDRESS L"info%40kalx.net"
#endif

// Put in docs directory for GitHub if defined
#ifdef SHFB_DOCS
#undef SHFB_DOCS
#ifdef _WIN64
#define SHFB_DOCS L"..\\..\\docs\\"
#else
#define SHFB_DOCS L"..\\docs\\"
#endif
#endif // SHFB_DOCS

namespace shfb {
    // help create MAML compliant strings
    class maml {
        std::wstring s;
    public:
        operator const std::wstring&() const
        {
            return s;
        }
        // append string
        maml& _(const std::wstring& str)
        {
            s.append(str);

            return *this;
        }
        maml& tag(const std::wstring& tag, const std::wstring& content)
        {
            static std::wstring lt(L"<"), gt(L">");

            s += lt + tag + gt + content + lt + L"/" + tag + gt;

            return *this;
        }
        maml& bold(const std::wstring& str)
        {
            return tag(L"legacyBold", str);
        }
        maml& c(const std::wstring& str)
        {
            return tag(L"codeInline", str);
        }
        maml& math(const std::wstring& str)
        {
            return tag(L"math", str);
        }
        // italic, underline
        maml& para(const std::wstring& str)
        {
            return tag(L"para", str);
        }
        maml& sub(const std::wstring& str)
        {
            return tag(L"subscript", str);
        }
        maml& sup(const std::wstring& str)
        {
            return tag(L"superscript", str);
        }
        // italic, underline


    };
};