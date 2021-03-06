/*  XmlLabelCreator.cpp
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

#include "XmlLabelCreator.h"

#include "EnumUtil.h"
#include "LabelModel.h"
#include "LabelModelObject.h"
//#include "LabelModelBarcodeObject.h"
#include "LabelModelBoxObject.h"
#include "LabelModelEllipseObject.h"
#include "LabelModelLineObject.h"
#include "LabelModelImageObject.h"
#include "LabelModelTextObject.h"
#include "DataCache.h"
#include "XmlTemplateCreator.h"
#include "XmlUtil.h"

#include "Merge/None.h"

#include <QByteArray>
#include <QFile>
#include <QTextBlock>
#include <QTextDocument>
#include <QBuffer>
#include <QtDebug>


namespace glabels
{

	void
	XmlLabelCreator::writeFile( const LabelModel* label, const QString& fileName )
	{
		QDomDocument doc;

		createDoc( doc, label );
		QByteArray buffer = doc.toByteArray( 2 );

		QFile file( fileName );

		if ( !file.open( QFile::WriteOnly | QFile::Text) )
		{
			qWarning() << "Error: Cannot write file " << fileName
			           << ": " << file.errorString();
		}

		file.write( buffer.data(), buffer.size() );
	}


	void
	XmlLabelCreator::writeBuffer( const LabelModel* label, QByteArray& buffer )
	{
		QDomDocument doc;

		createDoc( doc, label );
		buffer = doc.toByteArray( 2 );
	}


	void
	XmlLabelCreator::serializeObjects( const QList<LabelModelObject*>& objects,
	                                   QByteArray&                     buffer )
	{
		QDomDocument doc;

		QDomNode xmlNode( doc.createProcessingInstruction( "xml", "version=\"1.0\"" ) );
		doc.appendChild( xmlNode );

		QDomElement root = doc.createElement( "Glabels-objects" );
		doc.appendChild( root );
		XmlUtil::setStringAttr( root, "version", "4.0" );

		createDataNode( root, objects );
		createObjectsNode( root, objects, false );

		buffer = doc.toByteArray( 2 );
	}


	void
	XmlLabelCreator::createDoc( QDomDocument& doc, const LabelModel* label )
	{
		QDomNode xmlNode( doc.createProcessingInstruction( "xml", "version=\"1.0\"" ) );
		doc.appendChild( xmlNode );

		QDomElement root = doc.createElement( "Glabels-document" );
		doc.appendChild( root );
		XmlUtil::setStringAttr( root, "version", "4.0" );

		XmlTemplateCreator().createTemplateNode( root, label->tmplate() );

		createObjectsNode( root, label->objectList(), label->rotate() );

		if ( label->merge() && !dynamic_cast<merge::None*>(label->merge()) )
		{
			createMergeNode( root, label );
		}

		createDataNode( root, label->objectList() );
	}


	void
	XmlLabelCreator::createObjectsNode( QDomElement &parent, const QList<LabelModelObject*>& objects, bool rotate )
	{
		QDomDocument doc = parent.ownerDocument();
		QDomElement node = doc.createElement( "Objects" );
		parent.appendChild( node );

		XmlUtil::setStringAttr( node, "id", "0" );
		XmlUtil::setBoolAttr( node, "rotate", rotate );

		foreach ( LabelModelObject* object, objects )
		{
			if ( LabelModelBoxObject* boxObject = dynamic_cast<LabelModelBoxObject*>(object) )
			{
				createObjectBoxNode( node, boxObject );
			}
			else if ( LabelModelEllipseObject* ellipseObject = dynamic_cast<LabelModelEllipseObject*>(object) )
			{
				createObjectEllipseNode( node, ellipseObject );
			}
			else if ( LabelModelLineObject* lineObject = dynamic_cast<LabelModelLineObject*>(object) )
			{
				createObjectLineNode( node, lineObject );
			}
			else if ( LabelModelImageObject* imageObject = dynamic_cast<LabelModelImageObject*>(object) )
			{
				createObjectImageNode( node, imageObject );
			}
			else if ( LabelModelTextObject* textObject = dynamic_cast<LabelModelTextObject*>(object) )
			{
				createObjectTextNode( node, textObject );
			}
			// TODO: other object types
			else
			{
				Q_ASSERT_X( false, "XmlLabelCreator::createObjectsNode", "Invalid object type." );
			}
		}
	}


	void
	XmlLabelCreator::createObjectBoxNode( QDomElement &parent, const LabelModelBoxObject* object )
	{
		QDomDocument doc = parent.ownerDocument();
		QDomElement node = doc.createElement( "Object-box" );
		parent.appendChild( node );

		/* position attrs */
		createPositionAttrs( node, object );

		/* size attrs */
		createSizeAttrs( node, object );

		/* line attrs */
		createLineAttrs( node, object );

		/* fill attrs */
		createFillAttrs( node, object );

		/* affine attrs */
		createAffineAttrs( node, object );

		/* shadow attrs */
		createShadowAttrs( node, object );
	}


	void
	XmlLabelCreator::createObjectEllipseNode( QDomElement &parent, const LabelModelEllipseObject* object )
	{
		QDomDocument doc = parent.ownerDocument();
		QDomElement node = doc.createElement( "Object-ellipse" );
		parent.appendChild( node );

		/* position attrs */
		createPositionAttrs( node, object );

		/* size attrs */
		createSizeAttrs( node, object );

		/* line attrs */
		createLineAttrs( node, object );

		/* fill attrs */
		createFillAttrs( node, object );

		/* affine attrs */
		createAffineAttrs( node, object );

		/* shadow attrs */
		createShadowAttrs( node, object );
	}


	void
	XmlLabelCreator::createObjectLineNode( QDomElement &parent, const LabelModelLineObject* object )
	{
		QDomDocument doc = parent.ownerDocument();
		QDomElement node = doc.createElement( "Object-line" );
		parent.appendChild( node );

		/* position attrs */
		createPositionAttrs( node, object );

		/* size attrs of line */
		XmlUtil::setLengthAttr( node, "dx", object->w() );
		XmlUtil::setLengthAttr( node, "dy", object->h() );

		/* line attrs */
		createLineAttrs( node, object );

		/* affine attrs */
		createAffineAttrs( node, object );

		/* shadow attrs */
		createShadowAttrs( node, object );
	}


	void
	XmlLabelCreator::createObjectImageNode( QDomElement &parent, const LabelModelImageObject* object )
	{
		QDomDocument doc = parent.ownerDocument();
		QDomElement node = doc.createElement( "Object-image" );
		parent.appendChild( node );

		/* position attrs */
		createPositionAttrs( node, object );

		/* size attrs */
		createSizeAttrs( node, object );

		/* file attrs */
		if ( object->filenameNode().isField() )
		{
			XmlUtil::setStringAttr( node, "src_field", object->filenameNode().data() );
		}
		else
		{
			XmlUtil::setStringAttr( node, "src", object->filenameNode().data() );
		}

		/* affine attrs */
		createAffineAttrs( node, object );

		/* shadow attrs */
		createShadowAttrs( node, object );
	}


	void
	XmlLabelCreator::createObjectBarcodeNode( QDomElement &parent, const LabelModelBarcodeObject* object )
	{
		// TODO
	}


	void
	XmlLabelCreator::createObjectTextNode( QDomElement &parent, const LabelModelTextObject* object )
	{
		QDomDocument doc = parent.ownerDocument();
		QDomElement node = doc.createElement( "Object-text" );
		parent.appendChild( node );

		/* position attrs */
		createPositionAttrs( node, object );

		/* size attrs */
		createSizeAttrs( node, object );

		/* color attr */
		if ( object->textColorNode().isField() )
		{
			XmlUtil::setStringAttr( node, "color_field", object->textColorNode().key() );
		}
		else
		{
			XmlUtil::setUIntAttr( node, "color", object->textColorNode().rgba() );
		}

		/* font attrs */
		XmlUtil::setStringAttr( node, "font_family", object->fontFamily() );
		XmlUtil::setDoubleAttr( node, "font_size", object->fontSize() );
		XmlUtil::setStringAttr( node, "font_weight", EnumUtil::weightToString( object->fontWeight() ) );
		XmlUtil::setBoolAttr( node, "font_italic", object->fontItalicFlag() );
		XmlUtil::setBoolAttr( node, "font_underline", object->fontUnderlineFlag() );

		/* text attrs */
		XmlUtil::setDoubleAttr( node, "line_spacing", object->textLineSpacing() );
		XmlUtil::setStringAttr( node, "align", EnumUtil::hAlignToString( object->textHAlign() ) );
		XmlUtil::setStringAttr( node, "valign", EnumUtil::vAlignToString( object->textVAlign() ) );

		/* affine attrs */
		createAffineAttrs( node, object );

		/* shadow attrs */
		createShadowAttrs( node, object );

		/* serialize text contents */
		QTextDocument document( object->text() );
		int nBlocks = document.blockCount();
		for ( int iBlock = 0; iBlock < nBlocks; iBlock++ )
		{
			createPNode( node, document.findBlockByNumber(iBlock).text() );
		}
	}


	void
	XmlLabelCreator::createPNode( QDomElement &parent, const QString& blockText )
	{
		QDomDocument doc = parent.ownerDocument();
		QDomElement node = doc.createElement( "p" );
		parent.appendChild( node );

		node.appendChild( doc.createTextNode( blockText ) );
	}


	void
	XmlLabelCreator::createPositionAttrs( QDomElement &node, const LabelModelObject* object )
	{
		XmlUtil::setLengthAttr( node, "x", object->x0() );
		XmlUtil::setLengthAttr( node, "y", object->y0() );
	}

	
	void
	XmlLabelCreator::createSizeAttrs( QDomElement &node, const LabelModelObject* object )
	{
		XmlUtil::setLengthAttr( node, "w", object->w() );
		XmlUtil::setLengthAttr( node, "h", object->h() );
	}

	
	void
	XmlLabelCreator::createLineAttrs( QDomElement &node, const LabelModelObject* object )
	{
		XmlUtil::setLengthAttr( node, "line_width", object->lineWidth() );
		if ( object->lineColorNode().isField() )
		{
			XmlUtil::setStringAttr( node, "line_color_field", object->lineColorNode().key() );
		}
		else
		{
			XmlUtil::setUIntAttr( node, "line_color", object->lineColorNode().rgba() );
		}
	}

	
	void
	XmlLabelCreator::createFillAttrs( QDomElement &node, const LabelModelObject* object )
	{
		if ( object->fillColorNode().isField() )
		{
			XmlUtil::setStringAttr( node, "fill_color_field", object->fillColorNode().key() );
		}
		else
		{
			XmlUtil::setUIntAttr( node, "fill_color", object->fillColorNode().rgba() );
		}
	}


	void
	XmlLabelCreator::createAffineAttrs( QDomElement &node, const LabelModelObject* object )
	{
		QMatrix a = object->matrix();
	
		XmlUtil::setDoubleAttr( node, "a0", a.m11() );
		XmlUtil::setDoubleAttr( node, "a1", a.m12() );
		XmlUtil::setDoubleAttr( node, "a2", a.m21() );
		XmlUtil::setDoubleAttr( node, "a3", a.m22() );
		XmlUtil::setDoubleAttr( node, "a4", a.dx() );
		XmlUtil::setDoubleAttr( node, "a5", a.dy() );
	}


	void
	XmlLabelCreator::createShadowAttrs( QDomElement &node, const LabelModelObject* object )
	{
		if ( object->shadow() )
		{
			XmlUtil::setBoolAttr( node, "shadow", object->shadow() );

			XmlUtil::setLengthAttr( node, "shadow_x", object->shadowX() );
			XmlUtil::setLengthAttr( node, "shadow_y", object->shadowY() );

			if ( object->fillColorNode().isField() )
			{
				XmlUtil::setStringAttr( node, "shadow_color_field", object->shadowColorNode().key() );
			}
			else
			{
				XmlUtil::setUIntAttr( node, "shadow_color", object->shadowColorNode().rgba() );
			}

			XmlUtil::setDoubleAttr( node, "shadow_opacity", object->shadowOpacity() );
		}
	}


	void
	XmlLabelCreator::createMergeNode( QDomElement &parent, const LabelModel* label )
	{
		QDomDocument doc = parent.ownerDocument();
		QDomElement node = doc.createElement( "Merge" );
		parent.appendChild( node );

		XmlUtil::setStringAttr( node, "type", label->merge()->id() );
		XmlUtil::setStringAttr( node, "src", label->merge()->source() );
	}


	void
	XmlLabelCreator::createDataNode( QDomElement &parent, const QList<LabelModelObject*>& objects )
	{
		QDomDocument doc = parent.ownerDocument();
		QDomElement node = doc.createElement( "Data" );
		parent.appendChild( node );

		DataCache data( objects );

		foreach ( QString name, data.imageNames() )
		{
			createPngFileNode( node, name, data.getImage( name ) );
		}

		foreach ( QString name, data.svgNames() )
		{
			createSvgFileNode( node, name, data.getSvg( name ) );
		}
	}


	void
	XmlLabelCreator::createPngFileNode( QDomElement &parent, const QString& name, const QImage& image )
	{
		QDomDocument doc = parent.ownerDocument();
		QDomElement node = doc.createElement( "File" );
		parent.appendChild( node );

		XmlUtil::setStringAttr( node, "name", name );
		XmlUtil::setStringAttr( node, "mimetype", "image/png" );
		XmlUtil::setStringAttr( node, "encoding", "base64" );

		QByteArray ba;
		QBuffer buffer(&ba);
		buffer.open(QIODevice::WriteOnly);
		image.save(&buffer, "PNG");
		QByteArray ba64 = ba.toBase64();

		node.appendChild( doc.createTextNode( QString( ba64 ) ) );
	}


	void
	XmlLabelCreator::createSvgFileNode( QDomElement &parent, const QString& name, const QByteArray& svg )
	{
		QDomDocument doc = parent.ownerDocument();
		QDomElement node = doc.createElement( "File" );
		parent.appendChild( node );

		XmlUtil::setStringAttr( node, "name", name );
		XmlUtil::setStringAttr( node, "mimetype", "image/svg+xml" );
		XmlUtil::setStringAttr( node, "encoding", "cdata" );

		node.appendChild( doc.createCDATASection( QString( svg ) ) );
	}

} // namespace glabels
