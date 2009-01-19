/*
QijMarkdown - A port of the Markdown text converter to Qt 4
Copyright (C) 2009 Matthew J Smith

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <QString>
#include <QRegExp>

QijMarkdown::QijMarkdown( QString &sourceText )
{
  myString = sourceText;

  myString.replace( "\r\n", "\n" ); // DOS to Unix
  myString.replace( '\r', "\n" ); // Mac to Unix

  myString.append( "\n\n" );     // Make sure the string ends with a couple of newlines
  detab( myString ); // Convert all tabs to spaces
  myString.remove( QRegExp( "^[ \t]+$" ) ); // Remove any lines consisting only of tabs and spaces

  hashHTMLBlocks( myString );
  stripLinkDefinitions( myString );
  runBlockGamut( myString );
  unescapeSpecialChars( myString );
}

inline static QString fromMarkdown( QString &s )
{
  return QijMarkdown( s ).getString();
}
