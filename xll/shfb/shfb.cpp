// shfb.cpp - Sandcastle Help File Builder
#include "../xll.h"

using namespace xll;

// xlfFwrite only writes 255 chars at a time.
inline OPER fwrite(const OPER& file, const OPER& text)
{
    OPER start{ 1 };
    OPER len = Excel(xlfLen, text);

    while (len > 255) {
        const OPER& mid = Excel(xlfMid, text, start, OPER(255));
        auto result = Excel(xlfFwrite, file, mid);
        ensure(result == 255);
        start = start + 255;
        len = len - 255;
    }

    return Excel(xlfFwrite, file, Excel(xlfMid, text, start, len));
}

// Read one line at a time.
inline OPER fread(const OPER& file)
{
    OPER fd = Excel(xlfFopen, file, OPER(3));
    ensure(fd.isNum());

    OPER text;
    while (OPER line = Excel(xlfFreadln, fd)) {
        text = text & line;
    }

    Excel(xlfFclose, fd);

    return text;
}

inline OPER find_last_of(const OPER& find, const OPER& within)
{
    OPER off{ 0 };

    while (auto next = Excel(xlfFind, find, within, OPER(off + 1))) {
        off = next;
    }

    return off;
}

inline OPER basename(const OPER& path, bool strip = false)
{
    OPER off = find_last_of(OPER(L"\\"), path);
    OPER base = Excel(xlfRight, path, OPER(Excel(xlfLen, path) - off));
    
    if (strip) {
        base = Excel(xlfLeft, base, OPER(find_last_of(OPER(L"."), base) - 1));
    }

    return base;
}

inline OPER dirname(const OPER& path)
{
    OPER off = find_last_of(OPER(L"\\"), path);
    
    return Excel(xlfLeft, path, off);
}

// guids based on strings
inline std::wstring make_guid(const std::wstring& s)
{
    wchar_t buf[32 + 4 + 1];

    std::hash<std::wstring> hash;
    swprintf(buf, 36, L"%08x-0000-0000-0000-000000000000", hash(s));

    return std::wstring(buf);
}

OPER content_layout(const OPER& base)
{
    OPER cl(
#include "Content Layout.content"
    );

    OPER TopicGUID = OPER(make_guid(base.to_string()));
    cl = Excel(xlfSubstitute, cl, OPER(L"{{TopicGUID}}"), TopicGUID);

    OPER Topics;
    //<Topic id = "d7e05719-f06e-4480-8f4a-e3ce3aeef4e0" visible = "True" / >
    OPER Pre(L"\n    <Topic id=\"");
    OPER Post(L"\" visible=\"true\" />");
    for (const auto& args : AddIn::map()) {
        if (args.second.Documentation() && args.second.isFunction()) {
            auto guid = make_guid(args.first.to_string());
            OPER GUID(guid.data(), guid.length());
            Topics = Topics & Pre & GUID & Post;
        }
    }
    cl = Excel(xlfSubstitute, cl, OPER(L"{{Topics}}"), Topics);

    return cl;
}
OPER template_shfbproj(const OPER& base)
{
    OPER tp(
#include "template.shfbproj"
    );
    tp = Excel(xlfSubstitute, tp, OPER(L"{{Base}}"), base);
    //<None Include = "Reference\FUNCTION.aml" / >
    OPER Pre = OPER(L"\n    <None Include=\"");
    OPER Post = OPER(L".aml\" />");
    OPER ItemGroup = Pre & base & Post;
    for (const auto& args : AddIn::map()) {
        if (args.second.Documentation() && args.second.isFunction())  {
            ItemGroup = ItemGroup & Pre & args.first & Post;
        }
    }
    tp = Excel(xlfSubstitute, tp, OPER(L"{{ItemGroup}}"), ItemGroup);

    return tp;
}

OPER documentation_aml(const OPER& name, const Args& args)
{
    OPER da(
#include "Documentation.aml"
    );

    OPER TopicId = OPER(make_guid(name.to_string()));
    da = Excel(xlfSubstitute, da, OPER(L"{{TopicId}}"), TopicId);
    if (args.Documentation()) {
        da = Excel(xlfSubstitute, da, OPER(L"{{Documentation}}"), OPER(args.Documentation()));
    }

    return da;
}

OPER function_aml(const OPER& name, const Args& args)
{
    OPER fa(
#include "Function.aml"
    );

    OPER TopicId = OPER(make_guid(name.to_string()));
    fa = Excel(xlfSubstitute, fa, OPER(L"{{TopicId}}"), TopicId);
    fa = Excel(xlfSubstitute, fa, OPER(L"{{FunctionHelp}}"), args.FunctionHelp());
    fa = Excel(xlfSubstitute, fa, OPER(L"{{Documentation}}"), OPER(args.Documentation()));
    fa = Excel(xlfSubstitute, fa, OPER(L"{{Syntax}}"), args.Syntax());
    
    OPER ListItems;
    OPER Pre(L"\n    <listItem><para>");
    OPER Post(L"\n    </para></listItem>");
    auto Bold = [](const OPER& arg) { return OPER(L"      <legacyBold>") & arg & OPER(L"</legacyBold>"); };
    for (int i = 1; i <= args.Arity(); ++i) {
        ListItems = ListItems & Pre
            & Bold(args.ArgumentText(i)) & OPER(L" ") & args.ArgumentHelp(i) & Post;
    }
    fa = Excel(xlfSubstitute, fa, OPER(L"{{ListItems}}"), ListItems);
    if (args.Remarks()) {
        OPER Remarks(L"<section address=\"Remarks\"><title>Remarks</title><content><para>");
        Remarks.append(args.Remarks());
        Remarks = Remarks & OPER(L"</para></content></section>");
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Remarks}}"), Remarks);
    }
    else {
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Remarks}}"), OPER(L""));
    }
    if (args.Examples()) {
        OPER Examples(L"<section address=\"Examples\"><title>Examples</title><content><para>");
        Examples.append(args.Examples());
        Examples = Examples & OPER(L"</para></content></section>");
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Examples}}"), Examples);
    }
    else {
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Examples}}"), OPER(L""));
    }


    return fa;
}

void make_shfb(const OPER& lib)
{
    OPER dir = dirname(lib);
    OPER base = basename(lib, true);
    //<Topic id = "d7e05719-f06e-4480-8f4a-e3ce3aeef4e0" visible = "True" / >

    //OPER s = L"={\"a\",1.2;\"b\", TRUE}";
    //OPER o = Excel(xlfEvaluate, s);

    OPER cl = Excel(xlfFopen, dir & OPER(L"Content Layout.content"), OPER(3));
    ensure(cl.isNum());
    fwrite(cl, content_layout(base));
    Excel(xlfFclose, cl);

    OPER tp = Excel(xlfFopen, dir & base & OPER(L".shfbproj"), OPER(3));
    ensure(tp.isNum());
    fwrite(tp, template_shfbproj(base));
    Excel(xlfFclose, tp);

    for (const auto& args : AddIn::map()) {
        if (args.second.isDocumentation()) {
            ensure(args.first == base);
            OPER fd = Excel(xlfFopen, dir & args.first & OPER(L".aml"), OPER(3));
            ensure(tp.isNum());
            fwrite(tp, documentation_aml(args.first, args.second));
            Excel(xlfFclose, tp);
        }
        else if (args.second.Documentation() && args.second.isFunction()) {
            OPER fd = Excel(xlfFopen, dir & args.first & OPER(L".aml"), OPER(3));
            ensure(tp.isNum());
            fwrite(tp, function_aml(args.first, args.second));
            Excel(xlfFclose, tp);
        }
    }
}

static AddIn xai_make_shfb(
    Macro(XLL_DECORATE(L"xll_make_shfb", 0), L"XLL.MAKE.SHFB")
);
extern "C" __declspec(dllexport) int WINAPI
xll_make_shfb(void)
{
    try {
        OPER lib = Args::XlGetName();
        make_shfb(lib);
        // get addin name
        // remove from dir
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return FALSE;
    }

    return TRUE;
}
Auto<OpenAfter> xao_make_doc(xll_make_shfb);
