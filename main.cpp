#
/*
 *    Copyright (C) 2010, 2011, 2012
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
 *	Main program
 */
#include	<QApplication>
#include	<QSettings>
#include	<QDir>
#ifdef	GUI_3
#include	<QCommandLineParser>
#endif
#include	<unistd.h>
#include	"dab-constants.h"
#include	"gui.h"

QString	fullPathfor (QString v) {
int16_t	i;
QString	fileName;

	if (v == QString ("")) 
	   return QString ("/tmp/xxx");

	if (v. at (0) == QChar ('/')) 		// full path specified
	   return v;

	fileName = QDir::homePath ();
	fileName. append ("/");
	fileName. append (v);
	fileName = QDir::toNativeSeparators (fileName);

	if (!fileName. endsWith (".ini"))
	   fileName. append (".ini");

	return fileName;
}

#define	DEFAULT_INI	".dab-rpi.ini"

int	main (int argc, char **argv) {
QString	initFileName;
RadioInterface	*MyRadioInterface;

// Default values
uint8_t		syncMethod	= 2;
QSettings	*dabSettings;		// ini file
uint8_t		dabMode		= 127;	// illegal value
QString		dabDevice	= QString ("");
QString		dabBand		= QString ("");
QString		ipAddress	= QString ("");
uint16_t	ipPort		= 1234;
//
//	Newer versions of Qt provide all kinds of nice mechanisms,
//	unfortunately, there are quite some people (including me (jvk))
//	who also work with older versions of Qt,
#if GUI_3
	QApplication a (argc, argv);
	QCoreApplication::setApplicationName("dab-rpi");
	QCoreApplication::setApplicationVersion(CURRENT_VERSION);

	QCommandLineParser optionParser;
	optionParser.setApplicationDescription("dab-rpi Help");
	optionParser.addHelpOption();
	optionParser.addVersionOption();

	QCommandLineOption INIFileOption ("i",
	          QCoreApplication::translate("main", "Settings INI-file path"),
	          QCoreApplication::translate("main", "Path"));
	optionParser.addOption(INIFileOption);

	QCommandLineOption SYNCOption ("S",
	          QCoreApplication::translate("main", "Sync method"),
	          QCoreApplication::translate("main", "Number"));
	optionParser.addOption(SYNCOption);
	QCommandLineOption InputOption ("D",
	          QCoreApplication::translate("main", "Input device"),
	          QCoreApplication::translate("main", "Name"));
	optionParser.addOption(InputOption);

	QCommandLineOption DABModeOption ("M",
	          QCoreApplication::translate("main", "DAB mode, possible are: 1,2 or 4, default: 1"),
	          QCoreApplication::translate("main", "Mode"));
	optionParser.addOption (DABModeOption);

	QCommandLineOption DABBandOption ("B",
	          QCoreApplication::translate("main", "DAB band"),
	          QCoreApplication::translate("main", "Band"));
	optionParser.addOption (DABBandOption);

	QCommandLineOption RTL_TCPServerIPOption ("I",
	          QCoreApplication::translate ("main", "rtl_tcp server IP address. Only valid for input rtl_tcp."),
	          QCoreApplication::translate ("main", "IP Address"));
	optionParser.addOption(RTL_TCPServerIPOption);

	QCommandLineOption RTL_TCPServerIPPort ("P",
	          QCoreApplication::translate("main", "rtl_tcp server IP port. Only valid for input rtl_tcp."),
	          QCoreApplication::translate ("main", "Port"));
	optionParser.addOption(RTL_TCPServerIPPort);

//	Process the actual command line arguments given by the user
	optionParser.process(a);

//	Process INI file option
	QString INIFileValue = optionParser.value(INIFileOption);
	if (INIFileValue != "")
	   initFileName = fullPathfor (INIFileValue);
	else
	   initFileName = fullPathfor (QString (DEFAULT_INI));

	dabSettings =  new QSettings (initFileName, QSettings::IniFormat);

//	Process Sync method option
	QString SYNCOptionValue = optionParser.value (SYNCOption);
	if (SYNCOptionValue != "")
	   syncMethod = SYNCOptionValue. toInt ();

//	Process input device option
	QString InputValue = optionParser. value (InputOption);
	if (InputValue != "")
	   dabDevice = InputValue;

//	Process DAB mode option
	QString DABModValue = optionParser.value(DABModeOption);
	if (DABModValue != "") {
	   dabMode	= DABModValue. toInt();
	   if (!(dabMode == 1) || (dabMode == 2) || (dabMode == 4))
	      dabMode = 1;
	}

//	Process DAB band option
	QString DABBandValue = optionParser.value (DABBandOption);
	if (DABBandValue != "")
	   dabBand = DABBandValue;

//	Process rtl_tcp server IP address option
	QString RTL_TCPServerIPValue =
	                    optionParser. value (RTL_TCPServerIPOption);
	if (RTL_TCPServerIPValue != "")
	   ipAddress = RTL_TCPServerIPValue;

//	Process rtl_tcp server IP portoption
	QString RTL_TCPServerPortValue =
	                    optionParser. value(RTL_TCPServerIPPort);
	if (RTL_TCPServerPortValue != "")
	   ipPort = RTL_TCPServerPortValue. toInt ();

//	Since we do not have the possibility in GUI_2 and GUI_3 to select
//	Mode, Band or Device, we create the possibility for
//	passing appropriate parameters to the command
//	Selections - if any - will be default for the next session

	if (dabMode == 127)
	   dabMode = dabSettings -> value ("dabMode", 1). toInt ();
	if (dabDevice == QString (""))
	   dabDevice = dabSettings -> value ("device", "dabstick"). toString ();
	if (dabBand == QString (""))
	   dabBand = dabSettings -> value ("band", "BAND III"). toString ();
//
//	For gui_3 the data of a possible rtl_tcp should be saved
	a.setWindowIcon(QIcon(":/QML/images/icon.png"));
	dabSettings -> beginGroup ("rtl_tcp_client");
	if (ipAddress != QString ("")) {
	   dabSettings -> setValue ("rtl_tcp_address", ipAddress);
	   dabSettings -> setValue ("rtl_tcp_port", ipPort);
	}
	dabSettings -> endGroup ();

	(void)syncMethod;
	dabSettings -> setValue ("dabMode",	dabMode);
	dabSettings -> setValue ("device",	dabDevice);
	dabSettings -> setValue ("band",	dabBand);
	dabSettings	-> sync ();
	MyRadioInterface = new RadioInterface (dabSettings,
                                               dabDevice,
                                               dabMode,
                                               dabBand);
#else	
//	For Qt 4 lovers and other simple people 
	int	opt;
	QString	channel		= "11C";
	QString	programName	= "Classic FM";
	int	gain		= 20;
	QString	dabChannel	= QString ("");
	QString	dabProgramName	= QString ("");
#ifdef	GUI_2
#ifndef	TCP_STREAMER
	QString	soundChannel	= "default";
	QString	dabSoundchannel	= QString ("");
#endif
#endif
	int	dabGain		= -1;
	while ((opt = getopt (argc, argv, "i:D:S:M:B:C:P:G:A:")) != -1) {
	   switch (opt) {
	      case 'i':
	         initFileName = fullPathfor (QString (optarg));
	         break;

	      case 'S':
	         syncMethod	= atoi (optarg);
	         break;

#if defined (GUI_2) | defined (GUI_4)
	      case 'D':
	         dabDevice = optarg;
	         break;

	      case 'M':
	         dabMode	= atoi (optarg);
	         if (!(dabMode == 1) || (dabMode == 2) || (dabMode == 4))
	            dabMode = 1; 
	         break;

	      case 'B':
	         dabBand 	= optarg;
	         break;

	      case 'I':
	         ipAddress	= optarg;
	         break;
#endif
#ifdef	GUI_2
	      case 'C':
	         dabChannel	= QString (optarg);
	         break;

	      case 'P':
	         dabProgramName	= QString (optarg);
	         break;

	      case 'G':
	         dabGain	= atoi (optarg);
	         break;
#ifndef	TCP_STREAMER
	      case 'A':
	         dabSoundchannel	= QString (optarg);
	         break;
#endif
#endif
	      default:
	         break;
	   }
	}

	if (initFileName == QString (""))
	   initFileName	= fullPathfor (QString (DEFAULT_INI));
	dabSettings =  new QSettings (initFileName, QSettings::IniFormat);

#if defined (GUI_2) | defined (GUI_4)
//	Since we do not have the possibility in GUI_2 to select
//	Mode, Band or Device, we create the possibility for
//	passing appropriate parameters to the command
//	Selections - if any - will be default for the next session

	if (dabMode == 127)
	   dabMode = dabSettings -> value ("dabMode", 1). toInt ();
	if (dabDevice == QString (""))
	   dabDevice = dabSettings -> value ("device", "dabstick"). toString ();
	if (dabBand == QString (""))
	   dabBand = dabSettings -> value ("band", "BAND III"). toString ();
#endif 
#ifdef GUI_2
    	dabSettings -> beginGroup ("gui_2");
	if (dabChannel == QString (""))
	   channel = dabSettings -> value ("channel", channel). toString ();
	else
	   channel = dabChannel;
	if (dabProgramName == QString (""))
	   programName = dabSettings -> value ("programName",
	                                        programName). toString ();
	else
	   programName = dabProgramName;
	if (dabGain == -1)
	   gain	= dabSettings	-> value ("deviceGain", gain). toInt ();
	else
	   gain = dabGain;
#ifndef	TCP_STREAMER
	if (dabSoundchannel == QString (""))
	   soundChannel	=
	         dabSettings -> value ("soundChannel", soundChannel). toString ();
	else
	   soundChannel = dabSoundchannel;
#endif
	dabSettings	-> endGroup ();
#endif
/*
 *	Before we connect control to the gui, we have to
 *	instantiate
 */
	QApplication a (argc, argv);
#ifdef GUI_4
	(void)syncMethod;
	dabSettings -> setValue ("dabMode",	dabMode);
	dabSettings -> setValue ("device",	dabDevice);
	dabSettings -> setValue ("band",	dabBand);
	MyRadioInterface = new RadioInterface (dabSettings, 
	                                       dabDevice, dabMode, dabBand);
	dabSettings	-> sync ();
#elif GUI_2
	(void)syncMethod;
	dabSettings	-> setValue ("dabMode",	dabMode);
	dabSettings	-> setValue ("device",	dabDevice);
	dabSettings	-> setValue ("band",	dabBand);
	dabSettings	-> setValue ("channel",	channel);
	dabSettings	-> beginGroup ("gui_2");
	dabSettings	-> setValue ("programName",   programName);
	dabSettings	-> setValue ("deviceGain",  gain);
#ifndef	TCP_STREAMER
	dabSettings	-> setValue ("soundChannel", soundChannel);
#endif
	dabSettings	-> endGroup ();
	dabSettings	-> sync ();
	MyRadioInterface = new RadioInterface (dabSettings, 
	                                       dabDevice,
	                                       dabMode,
	                                       dabBand,
	                                       channel,
	                                       programName,
	                                       gain
#ifndef	TCP_STREAMER
	                                       , soundChannel
#endif
	                                       );
#elif GUI_1
	MyRadioInterface = new RadioInterface (dabSettings, syncMethod);
	MyRadioInterface -> show ();
#endif
#endif

#if QT_VERSION >= 0x050600
	QGuiApplication::setAttribute (Qt::AA_EnableHighDpiScaling);
#endif
	a. exec ();
/*
 *	done:
 */
	fflush (stdout);
	fflush (stderr);
	qDebug ("It is done\n");
	MyRadioInterface	-> ~RadioInterface ();
	dabSettings		-> ~QSettings ();
	exit (1);
}

