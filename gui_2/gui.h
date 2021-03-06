#
/*
 *    Copyright (C)  2010, 2011, 2012
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Programming
 *
 *    This file is part of the SDR-J.
 *    Many of the ideas as implemented in SDR-J are derived from
 *    other work, made available through the GNU general Public License. 
 *    All copyrights of the original authors are recognized.
 *
 *    SDR-J is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    SDR-J is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with SDR-J; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Gui design and implementation in gui_3:
 *	(c) Albrecht Lohoefener
 */

#ifndef _GUI
#define _GUI

#include	"dab-constants.h"
#include	<sndfile.h>

#include        <QByteArray>
#include        <QHostAddress>
#include        <QtNetwork>
#include        <QTcpSocket>
#include        <QTimer>

#include	"ofdm-processor.h"
#include	"ringbuffer.h"

class	QSettings;
class	virtualInput;
class	audioBase;

class	mscHandler;
class	ficHandler;

class	common_fft;

class RadioInterface: public QObject{
Q_OBJECT
public:
        	RadioInterface		(QSettings *,
	                                 QString,
	                                 uint8_t,
	                                 QString,
	                                 QString,	// channel
	                                 QString,	// program
	                                 int,		// gain
#ifndef	TCP_STREAMER
	                                 QString,	// sound channel
#endif
                                         QObject *parent = NULL);
		~RadioInterface		(void);

private:
	QSettings	*dabSettings;
	int16_t		threshold;
	void		setModeParameters	(uint8_t);

	DabParams	dabModeParameters;
	uint8_t		isSynced;
	uint8_t		dabBand;
	bool		running;
	virtualInput	*inputDevice;
	ofdmProcessor	*my_ofdmProcessor;
	ficHandler	*my_ficHandler;
	mscHandler	*my_mscHandler;
	audioBase	*soundOut;
	RingBuffer<int16_t>	*audioBuffer;
	bool		autoCorrector;
const	char		*get_programm_type_string (uint8_t);
const	char		*get_programm_language_string (uint8_t);
	void		dumpControlState	(QSettings *);

	QString		requestedProgram;
	QString		requestedChannel;
	QTimer		*successTimer;
	QString		ensemble;
	bool		isFICCRC;
	bool		isSignalPresent;
	int		coarseCorrector;
	int		fineCorrector;
	bool		setDevice		(QString);
	QString		stringFrom		(QByteArray);
	void		setStart		(void);
	void		TerminateProcess	(void);
	void		setChannel		(QString);
	void		setService		(QString);
	QStringList	stationList;

	int16_t		ficSuccess;
	int16_t		ficBlocks;
	
public slots:
	void		set_fineCorrectorDisplay	(int);
	void		set_coarseCorrectorDisplay	(int);
	void		clearEnsemble		(void);
	void		addtoEnsemble		(const QString &);
	void		nameofEnsemble		(int, const QString &);
	void		show_successRate	(int);
	void		show_ficCRC		(bool);
	void		show_snr		(int);
	void		setSynced		(char);
	void		showLabel		(QString);
	void		showMOT			(QByteArray, int, QString);
	void		sendDatagram		(char *, int);
	void		changeinConfiguration	(void);
	void		newAudio		(int);
//
	void		show_mscErrors		(int);
	void		show_ipErrors		(int);
	void		setStereo		(bool isStereo);

	void		autoCorrector_on	(void);
	void		showCorrectedErrors 	(int);
private slots:
	void		noSignalFound		(void);
};

#endif
