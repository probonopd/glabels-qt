/*  XmlLabelCreator.h
 *
 *  Copyright (C) 2014  Jim Evins <evins@snaught.com>
 *
 *  This file is part of gLabels-qt.
 *
 *  gLabels-qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gLabels-qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef XmlLabelCreator_h
#define XmlLabelCreator_h


#include <QObject>
#include <QDomElement>


namespace glabels
{

	// Forward references
	class LabelModel;
	class LabelModelObject;
	class LabelModelBoxObject;
	class LabelModelEllipseObject;
	class LabelModelLineObject;
	class LabelModelImageObject;
	class LabelModelBarcodeObject;
	class LabelModelTextObject;


	///
	/// XmlLabelCreator
	///
	class XmlLabelCreator : public QObject
	{
		Q_OBJECT

	public:
		static void writeFile( const LabelModel* label, const QString& fileName );
		static void writeBuffer( const LabelModel* label, QByteArray& buffer );
		static void serializeObjects( const QList<LabelModelObject*>& objects, QByteArray& buffer );

	private:
		static void createDoc( QDomDocument& doc, const LabelModel* label );
		static void createRootNode( const LabelModel* label );
		static void createObjectsNode( QDomElement &parent, const QList<LabelModelObject*>& objects, bool rotate );
		static void createObjectBoxNode( QDomElement &parent, const LabelModelBoxObject* object );
		static void createObjectEllipseNode( QDomElement &parent, const LabelModelEllipseObject* object );
		static void createObjectLineNode( QDomElement &parent, const LabelModelLineObject* object );
		static void createObjectImageNode( QDomElement &parent, const LabelModelImageObject* object );
		static void createObjectBarcodeNode( QDomElement &parent, const LabelModelBarcodeObject* object );
		static void createObjectTextNode( QDomElement &parent, const LabelModelTextObject* object );
		static void createPNode( QDomElement &parent, const QString& blockText );
		static void createPositionAttrs( QDomElement &node, const LabelModelObject* object );
		static void createSizeAttrs( QDomElement &node, const LabelModelObject* object );
		static void createLineAttrs( QDomElement &node, const LabelModelObject* object );
		static void createFillAttrs( QDomElement &node, const LabelModelObject* object );
		static void createAffineAttrs( QDomElement &node, const LabelModelObject* object );
		static void createShadowAttrs( QDomElement &node, const LabelModelObject* object );
		static void createMergeNode( QDomElement &parent, const LabelModel* label );
		static void createDataNode( QDomElement &parent, const QList<LabelModelObject*>& objects );
		static void createPngFileNode( QDomElement &parent, const QString& name, const QImage& image );
		static void createSvgFileNode( QDomElement &parent, const QString& name, const QByteArray& svg );

	};

}


#endif // XmlLabelCreator_h
