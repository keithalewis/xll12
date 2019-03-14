// shfb.cpp - Sandcastle Help File Builder
#include <string>
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

class xlfFile {
	OPER h; // file handle
public:
	xlfFile(const OPER& file)
		: h(Excel(xlfFopen, file, OPER(3)))
	{ }
	xlfFile(const xlfFile&) = delete;
	xlfFile& operator=(const xlfFile&) = delete;
	~xlfFile()
	{
		Excel(xlfFclose, h);
	}
	OPER read() const
	{
		return fread(h);
	}
	OPER write(const OPER& text) const
	{
		return fwrite(h, text);
	}
	OPER writeln(const OPER& text) const
	{
		ensure(Excel(xlfLen, text) < 256);

		return Excel(xlfFwriteln, h, text);
	}
};

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

inline OPER idh_safename(OPER x)
{
    for (int i = 1; i <= x.val.str[0]; ++i)
        if (!iswalnum(x.val.str[i]))
            x.val.str[i] = '_';

    return x;
}

OPER content_layout(const Args& args)
{
    OPER cl(
#include "Content Layout.content"
    );

    cl = Excel(xlfSubstitute, cl, OPER(L"{{TopicGUID}}"), args.Guid());

    OPER Topics;
    //<Topic id = "d7e05719-f06e-4480-8f4a-e3ce3aeef4e0" visible = "True" / >
    for (const auto& [key,arg] : AddIn::AddInMap()) {
        if (arg.Documentation() && arg.isFunction()) {
            OPER id(L"<Topic id=\"{{Guid}}\" visible=\"True\" title=\"{{Text}} function\" tocTitle=\"{{Text}}\" />");
            id = Excel(xlfSubstitute, id, OPER(L"{{Guid}}"), arg.Guid());
            id = Excel(xlfSubstitute, id, OPER(L"{{Text}}"), arg.FunctionText());
            Topics &= L"\n    ";
            Topics &= id;
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
    for (const auto& [key,arg] : AddIn::AddInMap()) {
        if (arg.Documentation() && *arg.Documentation()) {
            OPER name;
            if (arg.isDocumentation()) {
                name = base;
            }
            else if (arg.isFunction()) {
                name = arg.FunctionText();
            }
            ItemGroup = ItemGroup & Pre & name & Post;
        }
    }
    tp = Excel(xlfSubstitute, tp, OPER(L"{{ItemGroup}}"), ItemGroup);

    return tp;
}

OPER documentation_aml(const Args& args)
{
    OPER da(
#include "Documentation.aml"
    );

    da = Excel(xlfSubstitute, da, OPER(L"{{TopicId}}"), args.Guid());
    if (args.Documentation()) {
        da = Excel(xlfSubstitute, da, OPER(L"{{Documentation}}"), OPER(args.Documentation()));
    }

    return da;
}

OPER function_aml(const Args& args)
{
    OPER fa(
#include "Function.aml"
    );

    fa = Excel(xlfSubstitute, fa, OPER(L"{{TopicId}}"), args.Guid());
    fa = Excel(xlfSubstitute, fa, OPER(L"{{FunctionHelp}}"), args.FunctionHelp());
    fa = Excel(xlfSubstitute, fa, OPER(L"{{Documentation}}"), OPER(args.Documentation()));
    fa = Excel(xlfSubstitute, fa, OPER(L"{{Syntax}}"), args.Syntax());
    
    OPER ListItems;
    OPER Pre(L"\n    <listItem><para>");
    OPER Post(L"\n    </para></listItem>");
    auto Bold = [](const OPER& arg) { return OPER(L"      <legacyBold>") & arg & OPER(L"</legacyBold>"); };
    for (int i = 1; i <= args.Arity(); ++i) {
        ListItems = ListItems & Pre
            & Bold(args.ArgumentName(i)) & OPER(L" ") & args.ArgumentHelp(i) & Post;
    }
    fa = Excel(xlfSubstitute, fa, OPER(L"{{ListItems}}"), ListItems);
    if (args.Remarks() && *args.Remarks()) {
        OPER Remarks(L"<section address=\"Remarks\"><title>Remarks</title><content><para>");
        Remarks.append(args.Remarks(), wcslen(args.Remarks()));
        Remarks = Remarks & OPER(L"</para></content></section>");
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Remarks}}"), Remarks);
    }
    else {
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Remarks}}"), OPER(L""));
    }
    if (args.Examples() && *args.Examples()) {
        OPER Examples(L"<section address=\"Examples\"><title>Examples</title><content><para>");
        Examples &= args.Examples();
        Examples &= L"</para></content></section>";
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Examples}}"), Examples);
    }
    else {
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Examples}}"), OPER(L""));
    }

    return fa;
}

inline OPER alias_txt(const Args& arg)
{
    return OPER(L"IDH_") & arg.Key() & OPER(L"=html\\") & arg.Guid() & OPER(L".htm");
}

inline OPER map_h(const Args& arg)
{
    return OPER(L"#define IDH_") & arg.Key() & OPER(L" ") & arg.TopicId();
}

//!!! turn the Fopen - Fclose into an RAII class
void make_shfb(const OPER& lib)
{
    OPER dir = dirname(lib);
    OPER base = basename(lib, true); // strip off .xll
    //<Topic id = "d7e05719-f06e-4480-8f4a-e3ce3aeef4e0" visible = "True" / >

    //OPER s = L"={\"a\",1.2;\"b\", TRUE}";
    //OPER o = Excel(xlfEvaluate, s);

	xlfFile at(dir & OPER(L"alias.txt"));
    xlfFile mh(dir & OPER(L"map.h"));
    for (const auto& [key,arg] : AddIn::AddInMap()) {
        if (arg.Documentation() && *arg.Documentation()) {
            if (arg.isDocumentation()) {
                // Assumes only one documentation add-in.

                xlfFile cl(dir & OPER(L"Content Layout.content"));
                cl.write(content_layout(arg));

                xlfFile tp(dir & base & OPER(L".shfbproj"));
                tp.write(template_shfbproj(base));

                xlfFile fd(dir & base & OPER(L".aml"));
                fd.write(documentation_aml(arg));
            }
			else if (arg.isFunction()) {
				xlfFile fd(dir & arg.FunctionText() & OPER(L".aml"));
				fd.write(function_aml(arg));
			}
            // else if (arg.isMacro()) { ...
            at.writeln(alias_txt(arg));
            mh.writeln(map_h(arg));
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
#ifdef _DEBUG
Auto<OpenAfter> xao_make_doc(xll_make_shfb);
#endif