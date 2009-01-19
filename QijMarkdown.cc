// QijMarkdown.cc - Markdown port to Qt 4
// Written by Matthew J Smith
// Copyright (C) 2009, Matthew J Smith

#include <QString>
#include <QRegExp>

namespace QijMarkdown
{
  static QString fromMarkdown( QString &sourceText )
  {
    QString myString;

    myString = sourceText;

    myString.replace( "\r\n", "\n" ); // DOS to Unix
    myString.replace( '\r', "\n" ); // Mac to Unix

    myString.append( "\n\n" );     // Make sure the string ends with a couple of newlines
    myString = _Detab( myString ); // Convert all tabs to spaces
    myString.remove( QRegExp( "^[ \t]+$" ) ); // Remove any lines consisting only of tabs and spaces
    
    myString = _HashHTMLBlocks( myString );
    myString = _StripLinkDefinitions( myString );
    myString = _RunBlockGamut( myString );
    myString = _UnescapeSpecialChars( myString );
    
    return QString( "%1\n" ).arg( myString );
  }
}
