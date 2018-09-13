// doc.cpp - Create MAML documentation files
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#include "xll.h"

using namespace xll;

namespace {
	class scoped_handle {
		HANDLE h;
	protected:
		scoped_handle(HANDLE h)
			: h(h)
		{ }
	public:
		scoped_handle()
			: h(INVALID_HANDLE_VALUE)
		{ }
		scoped_handle(const scoped_handle&) = delete;
		scoped_handle& operator=(const scoped_handle&) = delete;
		~scoped_handle()
		{
            if (h != INVALID_HANDLE_VALUE) {
			    CloseHandle(h);
            }
		}
        operator HANDLE()
        {
            return h;
        }
	};
}

// guids based on strings
std::wstring make_guid(const std::wstring& s)
{
    wchar_t buf[32+4+1];

    std::hash<std::wstring> hash;
    swprintf(buf, 36, L"%8zu-0000-0000-0000-000000000000", hash(s));

    return std::wstring(buf);
}

inline OPER find_last_of(const OPER& find, const OPER& within)
{
    OPER off{ 0 };
    
    while (auto next = Excel(xlfFind, find, within, OPER(off + 1))) {
        off = next;
    }

    return off;
}

// Content Layout chunks
const OPER ProjectGuid(
    LR"shfb(
<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets = "Build" xmlns = "http://schemas.microsoft.com/developer/msbuild/2003" ToolsVersion = "4.0">
  <PropertyGroup>
    <Configuration Condition = " '$(Configuration)' == '' ">Debug</Configuration>
    <Platform Condition = " '$(Platform)' == '' ">AnyCPU</Platform>
    <SchemaVersion>2.0</SchemaVersion>
    <ProjectGuid>)shfb"
);

void make_shfbproj(/*const std::wstring& email = L"", const std::wstring& copy = L""*/)
{
    OPER lib = Args::XlGetName();

    auto off = find_last_of(OPER(L"\\"), lib);
    auto dir = Excel(xlfLeft, lib, off);
    auto file = Excel(xlfRight, lib, OPER(lib.size() - off));
    auto project = Excel(xlfLeft, file, OPER(find_last_of(OPER(L"."), file) - 1));
    auto aml = Excel(xlfConcatenate, dir, project, OPER(L".aml"));

    auto cl = Excel(xlfFopen, dir & OPER(L"Content Layout.content"), OPER(3));
    ensure(cl);
    Excel(xlfFwrite, cl, ProjectGuid);
    Excel(xlfFclose, cl);
#if 0
    os << LR"shfb(
<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets = "Build" xmlns = "http://schemas.microsoft.com/developer/msbuild/2003" ToolsVersion = "4.0">
  <PropertyGroup>
    <Configuration Condition = " '$(Configuration)' == '' ">Debug< / Configuration>
    <Platform Condition = " '$(Platform)' == '' ">AnyCPU< / Platform>
    <SchemaVersion>2.0< / SchemaVersion>
    <ProjectGuid>)shfb";
    
    os << make_guid(proj).c_str();

    os << LR"shfb(</ProjectGuid>
    <SHFBSchemaVersion>2017.9.26.0</SHFBSchemaVersion>
    <!-- SHFB properties -->
    <OutputPath>.\Debug\</OutputPath>
    <HtmlHelpName>)shfb";

    os << proj;

    os << LR"shfb(</HtmlHelpName>
    <Language>en-US</Language>
    <ApiFilter />
    <ComponentConfigurations />
    <HelpAttributes />
    <NamespaceSummaries />
    <PlugInConfigurations>
      <PlugInConfig id="Additional Content Only" enabled="True">
        <configuration />
      </PlugInConfig>
    </PlugInConfigurations>
    <BuildLogFile />
    <HtmlHelp1xCompilerPath />
    <HtmlHelp2xCompilerPath />
    <SandcastlePath />
    <WorkingPath />
    <BuildAssemblerVerbosity>OnlyWarningsAndErrors</BuildAssemblerVerbosity>
    <HelpFileFormat>HtmlHelp1, Website</HelpFileFormat>
    <IndentHtml>False</IndentHtml>
    <FrameworkVersion>.NET Framework 4.0</FrameworkVersion>
    <KeepLogFile>True</KeepLogFile>
    <DisableCodeBlockComponent>False</DisableCodeBlockComponent>
    <CppCommentsFixup>False</CppCommentsFixup>
    <CleanIntermediates>True</CleanIntermediates>
    <WebsiteSdkLinkType>None</WebsiteSdkLinkType>
    <HtmlSdkLinkType>Msdn</HtmlSdkLinkType>
    <IncludeFavorites>False</IncludeFavorites>
    <BinaryTOC>True</BinaryTOC>
    <SyntaxFilters>Visual Basic, Managed C++</SyntaxFilters>
    <SdkLinkTarget>Blank</SdkLinkTarget>
    <RootNamespaceContainer>False</RootNamespaceContainer>
    <PresentationStyle>VS2013</PresentationStyle>
    <Preliminary>False</Preliminary>
    <NamingMethod>Guid</NamingMethod>
    <HelpTitle>)shfb";

    os << L"The " << project.c_str() << L" add-in library";

    os << L"</HelpTitle>\n";
    
    if (email.length() > 0) {
        os << L"<FeedbackEMailAddress>" << email.c_str() << L"</FeedbackEMailAddress>\n";
    }
    
    os << LR"shfb(<ContentPlacement>AboveNamespaces</ContentPlacement>
        <HelpFileVersion>1.0.0.0</HelpFileVersion>
        <MaximumGroupParts>2</MaximumGroupParts>
        <NamespaceGrouping>False</NamespaceGrouping>
        <FooterText>
        </FooterText>
        <HeaderText>
        </HeaderText>
    )shfb";

    if (copy.length() > 0) {
        os << L"<CopyrightText>" << copy.c_str() << L"</CopyrightText>\n";
    }

    os << LR"shfb(
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' ">
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Release|AnyCPU' ">
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Debug|x86' ">
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Release|x86' ">
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Debug|x64' ">
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Release|x64' ">
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Debug|Win32' ">
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Release|Win32' ">
  </PropertyGroup>
  <ItemGroup>
    <ContentLayout Include="Content Layout.content" />
  </ItemGroup>
  <ItemGroup>
  )shfb";
    os << L"<None Include=\"" << project.c_str() << L".aml\" />\n";

    for (const auto& ai : AddIn::map()) {
        os << L"<None Include=\"" << ai.first << L".aml\" />\n";
    }

    os << LR"shfb(
  </ItemGroup>
  <!-- Import the SHFB build targets -->
  <Import Project="$(SHFBROOT)\SandcastleHelpFileBuilder.targets" />
</Project>
    )shfb";
#endif
}

void make_function(const Args& args)
{
    OPER function = args.FunctionText();
}

static AddIn xai_make_doc(
    Macro(XLL_DECORATE(L"xll_make_doc", 0), L"XLL.MAKE.DOC")
);
extern "C" __declspec(dllexport) int WINAPI
xll_make_doc(void)
{
    try {
        make_shfbproj();
	    // get addin name
	    // remove from dir
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());

        return FALSE;
    }

    return TRUE;
}
/*
FUNCTION function

Description
   ...

Syntax

FUNCTION(arg,...)

The DATEVALUE function syntax has the following arguments:

- *Arg* Argument help...

Remarks

...

Examples

...

*/