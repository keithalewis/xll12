// shfb.cpp - Sandcastle Help File Builder
#include <string>
#include "shfb.h"
#include "../xll.h"

using namespace xll;

OPER Remarks(LR"(
    <section address="Remarks">
      <title>Remarks</title>
      <content>
        <para>{{Remarks}}</para>
      </content>
    </section>
)");

OPER Examples(LR"(
    <section address="Examples">
      <title>Examples</title>
      <content>
        <para>{{Examples}}</para>
      </content>
    </section>
)");

// Replace with appropriate values or OPER() to remove.
static std::map<OPER, OPER> shfb_map = {
    { OPER(L"Organization"), OPER(SHFB_ORGANIZATION) },
{ OPER(L"FeedbackEMailAddress"), OPER(SHFB_FEEDBACKEMAILADDRESS) }
};

// xlfFwrite only writes 255 chars at a time.
inline OPER fwrite(const OPER& fd, const OPER& text)
{
    OPER start{ 1 };
    OPER len = Excel(xlfLen, text);

    while (len > 255) {
        const OPER& mid = Excel(xlfMid, text, start, OPER(255));
        auto result = Excel(xlfFwrite, fd, mid);
        ensure(result == 255);
        start = start + 255;
        len = len - 255;
    }

    return Excel(xlfFwrite, fd, Excel(xlfMid, text, start, len));
}

// Read one line at a time.
inline OPER fread(const OPER& fd)
{
    ensure(fd.isNum());

    OPER text;
    while (OPER line = Excel(xlfFreadln, fd)) {
        text = text & line;
    }

    return text;
}

class xlfFile {
    OPER h; // file handle
public:
    // 1 = read/write 2 = readonly 3 = create read/write
    xlfFile(const OPER& file, int access = 3)
        : h(Excel(xlfFopen, file, OPER(access)))
    { }
    xlfFile(const xlfFile&) = delete;
    xlfFile& operator=(const xlfFile&) = delete;
    ~xlfFile()
    {
        Excel(xlfFclose, h);
    }
    operator bool() const
    {
        return h.isNum();
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

    while (true) {
        const auto& next = Excel(xlfFind, find, within, OPER(off + 1));
        if (!next.isNum()) {
            break;
        }
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

OPER Tag(const wchar_t* tag, const OPER& attr) {
    return OPER(L"<") & OPER(tag) & OPER(L" ") & attr & OPER(L" />");
};

OPER Tag(const wchar_t* tag, const OPER& attr, const OPER& content)
{
    return OPER(L"<") & OPER(tag) & OPER(L" ") & attr & OPER(L">")
        & content & OPER(L"</") & OPER(tag) & OPER(L">");
}

OPER Attr(const wchar_t* name, const OPER& value) {
    return OPER(name) & OPER(L"=\"") & value & OPER(L"\" ");
};


OPER documentation_aml(const Args& args, const OPER& key)
{
    OPER da(
#include "Documentation.aml"
    );

    da = Excel(xlfSubstitute, da, OPER(L"{{TopicId}}"), Args::Guid(key));
    da = Excel(xlfSubstitute, da, OPER(L"{{Summary}}"), args.FunctionHelp());
    da = Excel(xlfSubstitute, da, OPER(L"{{Documentation}}"), OPER(args.Documentation()));
 
    return da;
}

OPER function_aml(const Args& args, const OPER& key)
{
    OPER fa(
#include "Function.aml"
    );

    fa = Excel(xlfSubstitute, fa, OPER(L"{{TopicId}}"), Args::Guid(key));
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
    if (!args.Remarks().empty()) {
        OPER remarks = Excel(xlfSubstitute, Remarks, OPER(L"{{Remarks}}"), OPER(args.Remarks()));
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Remarks}}"), remarks);
    }
    else {
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Remarks}}"), OPER(L""));
    }
    if (!args.Examples().empty()) {
        OPER examples = Excel(xlfSubstitute, Examples, OPER(L"{{Examples}}"), OPER(args.Examples()));
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Examples}}"), examples);
    }
    else {
        fa = Excel(xlfSubstitute, fa, OPER(L"{{Examples}}"), OPER(L""));
    }

    return fa;
}

inline OPER alias_txt(const OPER& key)
{
    return OPER(L"IDH_") & idh_safename(key) & OPER(L"=html\\") & Args::Guid(key) & OPER(L".htm");
}

inline OPER map_h(const OPER& key)
{
    return OPER(L"#define IDH_") & idh_safename(key) & OPER(L" ") & Args::TopicId(key);
}

OPER content_layout(const OPER& base)
{
    OPER cl(
#include "Content Layout.content"
    );

    OPER visible = Attr(L"visible", OPER(L"true"));
    OPER isExpanded = Attr(L"isExpanded", OPER(L"true"));
    OPER isSelected = Attr(L"isSelected", OPER(L"true"));
 
    auto Topic = [](const OPER& attr) {
        return Tag(L"Topic", attr);
    };
    OPER Topics = Topic(Attr(L"id", Args::Guid(base)) &
                  visible & isExpanded & isSelected);

    std::map<OPER, OPER> topicMap;

    for (const auto& [key, arg] : AddIn::KeyArgsMap) {
        if (!arg.Documentation().empty()) {
            if (arg.isFunction()) {
                OPER category = arg.Category();
                //  title="FUNCTION function" tocTitle="FUNCTION" linkText="FUNCTION" />
                OPER& topicCategory = topicMap[category];
                topicCategory &= OPER(L"\n    ");
                OPER topic = Topic(
                    Attr(L"id", Args::Guid(key)) &
                    Attr(L"title", key));
                topicCategory &= topic;
            }
            // else if isMacro!!!
        }
    }

    for (const auto& [cat,topic] : topicMap) {
        OPER attr = Attr(L"id", Args::Guid(cat))
            & Attr(L"title", cat) & visible;
        Topics &= OPER(L"\n  ");
        Topics &= Tag(L"Topic", attr, topic);
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
#ifdef SHFB_ORGANIZATION
    OPER org(L"<CopyrightText>Copyright &amp;#169%3b {{Organization}}</CopyrightText>");
    org = Excel(xlfSubstitute, org, OPER(L"{{Organization}}"), OPER(SHFB_ORGANIZATION));
    tp = Excel(xlfSubstitute, tp, OPER(L"{{CopyrightText}}"), org);
#else
    tp = Excel(xlfSubstitute, tp, OPER(L"{{CopyrightText}}"), OPER(L""));
#endif
#ifdef SHFB_FEEDBACKEMAILADDRESS
    OPER email = OPER(L"<FeedbackEMailAddress>");
    email = email & OPER(SHFB_FEEDBACKEMAILADDRESS);
    email = email & OPER(L"</FeedbackEMailAddress>");
    tp = Excel(xlfSubstitute, tp, OPER(L"{{FeedbackEMailAddress}}"), email);
#else
    tp = Excel(xlfSubstitute, tp, OPER(L"{{FeedbackEMailAddress}}"), OPER(L""));
#endif

    //<None Include = "Reference\FUNCTION.aml" / >
    OPER Pre = OPER(L"\n    <None Include=\"");
    OPER Post = OPER(L".aml\" />");
    OPER ItemGroup = Pre & base & Post;
    OPER Pres = OPER(L"\n   <Folder Include=\"");
    OPER Posts = OPER(L"\" />");
    OPER ItemGroups;
    std::set<OPER> igs;

    for (const auto& [key, arg] : AddIn::KeyArgsMap) {
        if (!arg.Documentation().empty()) {
            OPER name;
            if (arg.isDocument()) {
                name = base;
            }
            else if (arg.isFunction()) {
                if (arg.Category().size() > 0) {
                    igs.insert(arg.Category());
                }
                name = key;
            }
            ItemGroup = ItemGroup & Pre & name & Post;
        }
    }
    
    for (const OPER& ig : igs) {
        ItemGroup = ItemGroup & Pre & ig & Post;
        ItemGroups = ItemGroups & Pres & ig & Posts;
    }
    tp = Excel(xlfSubstitute, tp, OPER(L"{{ItemGroup}}"), ItemGroup);
    tp = Excel(xlfSubstitute, tp, OPER(L"{{ItemGroups}}"), ItemGroups);
    

    return tp;
}

void make_shfb(const OPER& lib)
{
    OPER dir = dirname(lib);
    OPER base = basename(lib, true); // strip off .xll
#ifdef SHFB_DOCS
    dir = dir & SHFB_DOCS;
    dir = dir & base;
    dir = dir & L"\\";
    CreateDirectory(dir, NULL);
#endif // SHFB_DOCS
    //<Topic id = "d7e05719-f06e-4480-8f4a-e3ce3aeef4e0" visible = "True" / >

    /*
    // Create documentation if it does not exist.
    if (AddIn::KeyArgsMap.find(OPER(L"")) == AddIn::KeyArgsMap.end()) {
        std::wstring base_help = std::wstring(L"The ") + base.toStr() + L" add-in.";
        std::wstring base_doc = std::wstring(L"Documentation for the ") + base.toStr() + L" Excel add-in.";
        xll::AddIn xai_base(xll::Document(base_help.c_str()).Documentation(base_doc.c_str()));
    }
    */
    //OPER s = L"={\"a\",1.2;\"b\", TRUE}";
    //OPER o = Excel(xlfEvaluate, s);

    xlfFile tp(dir & base & OPER(L"_shfb.shfbproj"));
    tp.write(template_shfbproj(base));

    xlfFile cl(dir & OPER(L"Content Layout.content"));
    cl.write(content_layout(base));

    xlfFile at(dir & OPER(L"alias.txt"));
    xlfFile mh(dir & OPER(L"map.h"));
    for (const auto& [key, arg] : AddIn::KeyArgsMap) {
        if (!arg.Documentation().empty()) {
            if (arg.isDocument()) {
                // Assumes only one documentation add-in per category.
                OPER cat = arg.Category();
                xlfFile fd(dir & cat & OPER(L".aml"));
                fd.write(documentation_aml(arg, cat));
            }
            else if (arg.isFunction()) {
                xlfFile fd(dir & key & OPER(L".aml"));
                fd.write(function_aml(arg, key));
                at.writeln(alias_txt(key));
                mh.writeln(map_h(key));
            }
            // else if (arg.isMacro()) { ...
        }
    }
}

static AddIn xai_make_shfb(
    Macro(XLL_DECORATE(L"xll_make_shfb", 0), L"MAKE.SHFB")
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
//Auto<OpenAfter> xao_make_doc(xll_make_shfb);
#endif