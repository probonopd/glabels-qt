/*  BarcodeMenu.cpp
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

#include "BarcodeMenu.h"

#include "BarcodeBackends.h"
#include "BarcodeMenuItem.h"


namespace glabels
{
	
	///
	/// Constructor
	///
	BarcodeMenu::BarcodeMenu()
	{
		foreach ( QString name, BarcodeBackends::getNameList() )
		{
			const BarcodeStyle* bcStyle = BarcodeBackends::lookupStyleFromName( name );

			BarcodeMenuItem* bcMenuItem = new BarcodeMenuItem( bcStyle );
			connect( bcMenuItem, SIGNAL(activated()), this, SLOT(onMenuItemActivated) );

			addAction( bcMenuItem );
		}
	}


	///
	/// bcStyle getter
	///
	const BarcodeStyle* BarcodeMenu::bcStyle() const
	{
		return mBcStyle;
	}


	///
	/// onMenuItemActivated slot
	///
	void BarcodeMenu::onMenuItemActivated( BarcodeStyle *bcStyle )
	{
		mBcStyle = bcStyle;

		emit styleChanged();
	}

} // namespace glabels
