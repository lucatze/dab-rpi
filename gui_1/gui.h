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
 */

#ifndef _GUI
#define _GUI

#include	"dab-constants.h"
#include	<QMainWindow>
#include	<QStringList>
#include	<QStringListModel>
#include	<QUdpSocket>
#include	<QComboBox>
#include	<QLabel>
#include	<sndfile.h>
#include	"ui_gui_1.h"
#include	<QTimer>
#include	"ofdm-processor.h"
#include	"ringbuffer.h"

class	QSettings;
class	virtualInput;
class	audioBase;

class	mscHandler;
class	ficHandler;

class	common_fft;

#ifdef	TCP_STREAMER
class	tcpStreamer;
#elif	RTP_STREAMER
class	rtpStreamer;
#endif
/*
 *	GThe main gui object. It inherits from
 *	QDialog and the generated form
 */
class RadioInterface: public QMainWindow,
		      private Ui_dab_rpi {
Q_OBJECT
public:
		RadioInterface		(QSettings	*,
	                                 uint8_t	freqsyncMethod,
	                                 QWidget *parent = NULL);
		~RadioInterface		();

private:
	QSettings	*dabSettings;
	bool		autoStart;
	int16_t		threshold;
	uint8_t		freqsyncMethod;
	int32_t		vfoFrequency;
	void		setupChannels		(QComboBox *s, uint8_t band);
	void		setModeParameters	(uint8_t);
	void		clear_showElements	(void);
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
	QLabel		*pictureLabel;
	bool		saveSlide;
	QUdpSocket	DSCTy_59_socket;
	QString		ipAddress;
	int32_t		port;
	bool		show_crcErrors;
	void		init_your_gui		(void);
	void		dumpControlState	(QSettings *);
	FILE		*crcErrors_File;
	bool		sourceDumping;
	SNDFILE		*dumpfilePointer;
	bool		audioDumping;
	SNDFILE		*audiofilePointer;
	QStringList	soundChannels;
	QStringListModel	ensemble;
	QStringList	Services;
	QString		ensembleLabel;
	QTimer		*displayTimer;
	int32_t		numberofSeconds;
	void		resetSelector		(void);
	int16_t		ficBlocks;
	int16_t		ficSuccess;
public slots:
	void	set_fineCorrectorDisplay	(int);
	void	set_coarseCorrectorDisplay	(int);
	void	clearEnsemble		(void);
	void	addtoEnsemble		(const QString &);
	void	nameofEnsemble		(int, const QString &);
	void	show_successRate	(int);
	void	show_ficCRC		(bool);
	void	show_snr		(int);
	void	setSynced		(char);
	void	showLabel		(QString);
	void	showMOT			(QByteArray, int, QString);
	void	sendDatagram		(char *, int);
	void	changeinConfiguration	(void);
	void	newAudio		(int);
//
	void	show_mscErrors		(int);
	void	show_ipErrors		(int);
	void	setStereo		(bool);
	void	set_streamSelector	(int);
private slots:
//
//	Somehow, these must be connected to the GUI
//	We assume that any GUI will need these three:
	void	setStart		(void);
	void	TerminateProcess	(void);
	void	set_channelSelect	(QString);
	void	updateTimeDisplay	(void);

	void	autoCorrector_on	(void);

	void	set_modeSelect		(const QString &);
	void	set_bandSelect		(QString);
	void	setDevice		(QString);
	void	selectService		(QModelIndex);
	void	set_dumping		(void);
	void	set_audioDump		(void);
};

#endif

