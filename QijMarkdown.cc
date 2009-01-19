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
  g_tab_width = 4;
  emptyElementSuffix = " />"; // Change to " >" for old HTML rather than XHTML

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

inline QString QijMarkdown::toString()
{
  return myString;
}

inline static QString fromMarkdown( QString &s )
{
  return QijMarkdown( s ).getString();
}

void QijMarkdown::stripLinkDefinitions( QString &thisString )
{
  int lessThanTab = g_tab_width - 1;

  
}

void QijMarkdown::runBlockGamut( QString &thisString )
{
  doHeaders( thisString );
  
  // Do horizontal rules
  QList<QRegExp> regs;
  regs << QRegExp( "^[ ]{0,2}([ ]?\*[ ]?){3,}[ \t]*$" )
       << QRegExp( "^[ ]{0,2}([ ]? -[ ]?){3,}[ \t]*$" )
       << QRegExp( "^[ ]{0,2}([ ]? _[ ]?){3,}[ \t]*$" );

  Q_FOREACH( QRegExp rx, regs )
    myString.replace( rx, QString( "\n<hr%1\n" ).arg( emptyElementSuffix ) );
  
  doLists( myString );
  doCodeBlocks( myString );
  doBlockQuotes( myString );

  // Re-hash HTML blocks to avoid putting para tags round blocks
  hashHTMLBlocks( myString );
  
  formParagraphs( myString );
}

void QijMarkdown::runSpanGamut( QString &thisString )
{
  doCodeSpans( thisString );
  escapeSpecialChars( thisString );
  doImages( thisString );
  doAnchors( thisString );
  
  // Make links out of things like `<http://example.com/>`
	// Must come after _DoAnchors(), because you can use < and >
	// delimiters in inline links like [this](<url>).
  doAutoLinks( thisString );
  encodeAmpsAndAngles( thisString );
  doItalicsAndBold( thisString );

  // Do hard breaks
  thisString.replace( QRegExp( "{2,}\\n" ), QString( "<br%1\n" ).arg( emptyElementSuffix ) );
}

