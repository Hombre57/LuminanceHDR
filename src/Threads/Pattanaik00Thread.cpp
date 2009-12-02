/*
 * This file is a part of LuminanceHDR package.
 * ---------------------------------------------------------------------- 
 * Copyright (C) 2006,2007 Giuseppe Rota
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ---------------------------------------------------------------------- 
 *
 * Original Work
 * @author Giuseppe Rota <grota@users.sourceforge.net>
 * Improvements, bugfixing 
 * @author Franco Comida <fcomida@users.sourceforge.net>
 *
 */

#include "Common/config.h"
#include "Fileformat/pfsoutldrimage.h"
#include "Pattanaik00Thread.h"

void pfstmo_pattanaik00 (pfs::Frame*,bool,float,float,float,bool, ProgressHelper *);

Pattanaik00Thread::Pattanaik00Thread(pfs::Frame *frame, const TonemappingOptions &opts) : 
	TMOThread(frame, opts) {
}

void Pattanaik00Thread::run() {
	connect(ph, SIGNAL(valueChanged(int)), this, SIGNAL(setValue(int)));
	emit setMaximumSteps(100);
	try {
		pfstmo_pattanaik00(workingframe,
		opts.operator_options.pattanaikoptions.local,
		opts.operator_options.pattanaikoptions.multiplier,
		opts.operator_options.pattanaikoptions.cone,
		opts.operator_options.pattanaikoptions.rod,
		opts.operator_options.pattanaikoptions.autolum,ph);
	}
	catch(pfs::Exception e) {
		emit tmo_error(e.getMessage());
		emit deleteMe(this);
		return;
	}
	catch(...) {
		emit tmo_error("Failed to tonemap image");
		emit deleteMe(this);
		return;
	}
	
	if (!(ph->isTerminationRequested())) {
		const QImage& res = fromLDRPFStoQImage(workingframe);
		emit imageComputed(res);
	}
	emit finished();
	emit deleteMe(this);
}
//
// run()
//
