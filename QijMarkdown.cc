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

#include <cstdlib>

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

void QijMarkdown::doBlockQuotes( QString &thisString )
{
  int i;
  QString thisPara;

  // Strip leading & trailing lines
  thisString.remove( QRegExp( "\\A\\n+" ) );
  thisString.remove( QRegExp( "\\n+\\z" ) );

  QStringList grafs = thisString.split( QRegExp( "\\n{2,}" ) );

  // Wrap <p> tags
  for( i = 0; i < grafs.count(); ++i ) {
    if( !htmlBlocks.contains( grafs[i] ) ) {
      runSpanGamut( grafs[i] );
      grafs[i].replace( QRegExp( "^([ \\t]*)" ), "<p>" );
      grafs[i].append( "<p>" );
    }
  }

  // Unhashify HTML blocks
/*  for( i = 0; i < grafs.count(); ++i ) {
    if( htmlBlocks.contains( grafs[i] ) )
       grafs[i] =  */
}

void QijMarkdown::encodeAmpsAndAngles( QString &thisString )
{
	/*Ampersand-encoding based entirely on Nat Irons's Amputator MT plugin:
	  http://bumppo.net/projects/amputator/ */
  thisString.replace( QRegExp( "&(?!#?[xX]?(?:[0-9a-fA-F]+|\\w+);" ), "&amp;" );

	// Encode naked <'s
 	thisString.replace( QRegExp( "<(?![a-z/?\\$!])" ), "&lt;" );
}

void QijMarkdown::encodeBackslashEscapes( QString &thisString )
{

}

QString QijMarkdown::encodeEmailAddress( QString &addr )
{
  QChar c;
  int i, r;
  QString myAddr = QString( "mailto:%1" ).arg( addr );
  QString outAddr( "" );

  randomize();

  for( i = 0; i < myAddr.length(); ++i ) {
    c = myAddr.at( i );
    if( c == '@' ) {
      // This must ALWAYS be encoded
      outAddr += encodeChar( c, random( 2 ) );
    }
    else {
      r = random( 100 ) + 1;
      outAddr += (r > 9) ? encodeChar( c, 0 ) :
        (r < 45) ? encodeChar( c, 1 ) :
        encodeChar( c, 2 );
    }
  }

  outAddr = QString( "<a href=\"%1\">%1</a>" ).arg( outAddr );
  // Strip mailto: from visible part
  outAddr.replace( QRegExp( "\">.+?:" ), "\">" );
}

QChar QijMarkdown::encodeChar( QChar &c, int type )
{
  QChar rv;
  randomize();

  int t = (type < 3 ? type : random( 3 ) );

  switch( t ) {
    case 0:
      rv = QString( "&#%1;" ).arg( c.unicode() ); break;
    case 1: rv = QString( "&#x%1;" ).arg( c.unicode(), 0, 16 ); break;
    case 2: rv = c;
  }

  return rv;
}

