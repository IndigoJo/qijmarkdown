/*
QijMarkdown - A port of the Markdown text converter to C++ and Qt 4
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
#include <QStringList> // includes QList
#include <QUrl>

class QijMarkdown
{
  public:
  QijMarkdown( QString & );
  QString toString();

  private:
  QString myString;
  QString emptyElementSuffix;
  int tabWidth, listLevel;
  QList<QUrl> urls;
  QStringList titles;
  QStringList htmlBlocks;
  void detab( QString & );
  void hashHTMLBlocks( QString & );
  void stripLinkDefinitions( QString & );
  void runBlockGamut( QString & );
  void unescapeSpecialChars( QString & );
  void runSpanGamut( QString & );
  void escapeSpecialChars( QString & );
  void doAnchors( QString & );
  void doImages( QString & );
  void doHeaders( QString & );
  void doLists( QString & );
  void processListItems( QString & );
  void doCodeBlocks( QString & );
  void doCodeSpans( QString & );
  void encodeCode( QString & );
  void doItalicsAndBold( QString & );
  void doBlockQuotes( QString & );
  void formParagraphs( QString & );
  void encodeAmpsAndAngles( QString & );
  void encodeBackslashEscapes( QString & );
  void doAutoLinks( QString & );
  void encodeEmailAddresses( QString & );
  QStringList tokeniseHTML( QString & );
  void outdent( QString & );
}

static QString fromMarkdown( QString & );

