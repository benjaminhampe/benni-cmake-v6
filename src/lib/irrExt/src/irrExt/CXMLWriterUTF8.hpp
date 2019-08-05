// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_XML_WRITERUTF8_H_INCLUDED__
#define __C_XML_WRITERUTF8_H_INCLUDED__

#include <IXMLWriter.h>
#include <IWriteFile.h>
#include <irrExt/ustring.hpp>

namespace irr
{
namespace io
{

    //! Interface providing methods for making it easier to write XML files.
    class CXMLWriterUTF8 : public IXMLWriter
    {
    public:

        //! Constructor
        CXMLWriterUTF8(IWriteFile* file);

        //! Destructor
        virtual ~CXMLWriterUTF8();

        //! Writes a xml 1.0 header like <?xml version="1.0"?>
        virtual void writeXMLHeader();

        //! Writes an xml element with maximal 5 attributes
        virtual void writeElement(const wchar_t* name, bool empty=false,
            const wchar_t* attr1Name = nullptr, const wchar_t* attr1Value = nullptr,
            const wchar_t* attr2Name = nullptr, const wchar_t* attr2Value = nullptr,
            const wchar_t* attr3Name = nullptr, const wchar_t* attr3Value = nullptr,
            const wchar_t* attr4Name = nullptr, const wchar_t* attr4Value = nullptr,
            const wchar_t* attr5Name = nullptr, const wchar_t* attr5Value = nullptr);

        //! Writes an xml element with any number of attributes
        virtual void writeElement(const wchar_t* name, bool empty,
                core::array<core::stringw> &names, core::array<core::stringw> &values);

        //! Writes a comment into the xml file
        virtual void writeComment(const wchar_t* comment);

        //! Writes the closing tag for an element. Like </foo>
        virtual void writeClosingTag(const wchar_t* name);

        //! Writes a text into the file. All occurrences of special characters like
        //! & (&amp;), < (&lt;), > (&gt;), and " (&quot;) are automaticly replaced.
        virtual void writeText(const wchar_t* text);

        //! Writes a line break
        virtual void writeLineBreak();

        struct XMLSpecialCharacters
        {
            wchar_t Character;
            const wchar_t* Symbol;
        };

    private:

        void writeAttribute(const wchar_t* att, const wchar_t* name);

        IWriteFile* File;
        s32 Tabs;

        bool TextWrittenLast;
    };

} // end namespace irr
} // end namespace io

#endif
