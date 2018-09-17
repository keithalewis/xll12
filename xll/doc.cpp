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

// Can only write 255 chars.
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

// Content Layout chunks
const OPER ProjectGuid(
    LR"shfb(<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets = "Build" xmlns = "http://schemas.microsoft.com/developer/msbuild/2003" ToolsVersion = "4.0">
  <PropertyGroup>
    <Configuration Condition = " '$(Configuration)' == '' ">Debug</Configuration>
    <Platform Condition = " '$(Platform)' == '' ">AnyCPU</Platform>
    <SchemaVersion>2.0</SchemaVersion>
    <ProjectGuid>)shfb"
);

const OPER HtmlHelpName(
    LR"shfb(</ProjectGuid>
    <SHFBSchemaVersion>2017.9.26.0</SHFBSchemaVersion>
    <!-- SHFB properties -->
    <OutputPath>.\Debug\</OutputPath>
    <HtmlHelpName>)shfb"
);

const OPER HelpTitle(
    LR"shfb(</HtmlHelpName>
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
    <HelpTitle>)shfb"
);

const OPER ItemGroup(
    LR"shfb(</HelpTitle>
    <FeedbackEMailAddress>???</FeedbackEMailAddress>
    <ContentPlacement>AboveNamespaces</ContentPlacement>
    <HelpFileVersion>1.0.0.0</HelpFileVersion>
    <MaximumGroupParts>2</MaximumGroupParts>
    <NamespaceGrouping>False</NamespaceGrouping>
    <FooterText>
    </FooterText>
    <HeaderText>
    </HeaderText>
    <CopyrightText>???</CopyrightText>
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
    <None Include=")shfb"
);

const OPER Project(
    LR"shfb(</ItemGroup>
  <!-- Import the SHFB build targets -->
  <Import Project="$(SHFBROOT)\SandcastleHelpFileBuilder.targets" />
</Project>
    )shfb"
);

void make_shfbproj(/*const std::wstring& email = L"", const std::wstring& copy = L""*/)
{
    OPER lib = Args::XlGetName();

    OPER off = find_last_of(OPER(L"\\"), lib);
    OPER dir = Excel(xlfLeft, lib, off);
    OPER file = Excel(xlfRight, lib, OPER(lib.size() - off));
    OPER project = Excel(xlfLeft, file, OPER(find_last_of(OPER(L"."), file) - 1));
    OPER aml = Excel(xlfConcatenate, dir, project, OPER(L".aml"));

    OPER fd = Excel(xlfFopen, dir & project & OPER(L".shfbproj"), OPER(3));
    ensure(fd.isNum());
    
    fwrite(fd, ProjectGuid);
    Excel(xlfFwrite, fd, OPER(make_guid(std::wstring(aml.val.str + 1, aml.val.str[0]))));
    
    fwrite(fd, HtmlHelpName);
    Excel(xlfFwrite, fd, project);

    fwrite(fd, HelpTitle);
    Excel(xlfFwrite, fd, OPER(L"The ") & project & OPER(L" add-in library"));

    fwrite(fd, ItemGroup);
    Excel(xlfFwrite, fd, project & OPER(L".aml\" />\n"));

    for (const auto& ai : AddIn::map()) {
        if (ai.second.Documentation()) {
            Excel(xlfFwrite, fd, OPER(L"<None Include=\"") & ai.first & OPER(L".aml\" />\n"));
        }
    }

    fwrite(fd, Project);

    Excel(xlfFclose, fd);
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
Auto<Open> xao_make_doc(xll_make_doc);
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