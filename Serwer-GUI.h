/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/

#pragma once

#include "Socket.h"
#include "stdafx.h"
#include "Wykres.h"
#include "armax.h"
#include "wyjatki.h"
#include "IRegulator.h"
#include "RegulatorP.h"
#include "RegulatorPID.h"
#include "AdaptPID.h"
#include "PetlaRegulacji.h"
#include "Identyfikacja.h"
#include "GPC.h"
#include "Serializacja.h"
#include "Autentykacja.h"
#include <sstream>

/**
	Przestrzen nazw PSS_LAB.
*/
namespace PSS_LAB {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Runtime::InteropServices;
	
	void terminateFunction()
	{
		MessageBox::Show("Terminate","Krytyczny b³¹d",MessageBoxButtons::OK);
		exit(1);
	}

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>

	/**
		Klasa GUI serwera .
		@author Adrian Nikiel
	*/
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			set_terminate(terminateFunction);
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			try
			{
				typRegulatora->SelectedIndex = 2;

				model = new armax("nowe");
				reg = new RegulatorP("regulatoryP.conf", "nowy");
				regPID = new RegulatorPID("regulatoryPID.conf", "test");
				regAPID = new AdaptPID(1, 2, 10, 3);
				regGPC = new GPC("regulatorGPC.conf", "jeden",1000);
				petla = new PetlaRegulacji(model, regPID);
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), 0.99, 1000);
				
				serwer = new ServerSocket();
				serwer->StartHosting( PORT );
				iloscKlientow = 0;
				
				autentyk = new Autentykacja("prawa_dostepu.txt");
				
				// wyœwietlenie parametrów obiektów
				wyswietlParametryP();
				wyswietlParametryPID();
				wyswietlParametryAPID();
				wyswietlParametryGPC();
				wyswietlParametryObiektu();

				krok = 0;
				krokGPC = 0;

				Thread^ acc = gcnew Thread( gcnew ThreadStart(this,&PSS_LAB::Form1::nowyKlient));
				acc->Start();

			}
			catch(OpenException& exc)
			{
				System::Console::WriteLine(exc.what());
				MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
				exit(1);
			}
			catch(ReadDataException& exc)
			{
				System::Console::WriteLine(exc.what());
				MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
				exit(1);
			}
			catch( SocketException& exc )  
			{
				System::Console::WriteLine(exc.what());
				MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
				exit(1);
			}
			catch(...)
			{
				System::Console::WriteLine("Nieznany b³¹d!");
				MessageBox::Show("Nieznany b³¹d!", "B³¹d", MessageBoxButtons::OK);
				exit(1);
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			
			//delete acc;
			//acc->Abort();
			exit(0);

		}
	private: PSS_LAB::Wykres^  wykres1;
	private: System::Windows::Forms::Button^  start;
	private: PSS_LAB::Wykres^  wykres2;
	private: System::Windows::Forms::CheckBox^  stop;
	private: System::Windows::Forms::Button^  reset;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TrackBar^  czasKrokuTrackBar;
	private: System::Windows::Forms::NumericUpDown^  czasKrokuNumericUpDown;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label29;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::ComboBox^  typRegulatora;
	private: System::Windows::Forms::TabPage^  tabPage28;
	private: System::Windows::Forms::GroupBox^  groupBox10;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Label^  label77;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button12;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Button^  button13;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::Label^  label78;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::Label^  label79;
	private: System::Windows::Forms::Button^  button14;
	private: System::Windows::Forms::Label^  label80;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::Button^  button15;
	private: System::Windows::Forms::Label^  label81;
	private: System::Windows::Forms::TextBox^  textBox7;
	private: System::Windows::Forms::Label^  label82;
	private: System::Windows::Forms::Button^  button16;
	private: System::Windows::Forms::Label^  label83;
	private: System::Windows::Forms::Button^  button17;
	private: System::Windows::Forms::Button^  button18;
	private: System::Windows::Forms::TextBox^  textBox8;
	private: System::Windows::Forms::TextBox^  textBox9;
	private: System::Windows::Forms::Button^  button19;
	private: System::Windows::Forms::TextBox^  textBox10;
	private: System::Windows::Forms::Button^  button20;
	private: System::Windows::Forms::TextBox^  textBox11;
	private: System::Windows::Forms::Label^  label84;
	private: System::Windows::Forms::Label^  label85;
	private: System::Windows::Forms::Label^  label86;
	private: System::Windows::Forms::Button^  button21;
	private: System::Windows::Forms::TextBox^  textBox12;
	private: System::Windows::Forms::Button^  button22;
	private: System::Windows::Forms::TextBox^  textBox13;
	private: System::Windows::Forms::Button^  button23;
	private: System::Windows::Forms::TextBox^  textBox14;
	private: System::Windows::Forms::Label^  label87;
	private: System::Windows::Forms::Label^  label88;
	private: System::Windows::Forms::Label^  label89;
	private: System::Windows::Forms::GroupBox^  groupBox11;
	private: System::Windows::Forms::Button^  button24;
	private: System::Windows::Forms::Label^  label90;
	private: System::Windows::Forms::TextBox^  textBox15;
	private: System::Windows::Forms::TabPage^  tabPage29;
	private: System::Windows::Forms::TabControl^  tabControl8;
	private: System::Windows::Forms::TabPage^  tabPage30;
	private: System::Windows::Forms::Button^  button25;
	private: System::Windows::Forms::Button^  button26;
	private: System::Windows::Forms::TextBox^  textBox16;
	private: System::Windows::Forms::Label^  label91;
	private: System::Windows::Forms::Button^  button27;
	private: System::Windows::Forms::TextBox^  textBox17;
	private: System::Windows::Forms::Label^  label92;
	private: System::Windows::Forms::TextBox^  textBox18;
	private: System::Windows::Forms::Label^  label93;
	private: System::Windows::Forms::Button^  button28;
	private: System::Windows::Forms::TabPage^  tabPage31;
	private: System::Windows::Forms::Button^  button29;
	private: System::Windows::Forms::GroupBox^  groupBox12;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::CheckBox^  checkBox4;
	private: System::Windows::Forms::CheckBox^  checkBox5;
	private: System::Windows::Forms::CheckBox^  checkBox6;
	private: System::Windows::Forms::TabControl^  tabControl9;
	private: System::Windows::Forms::TabPage^  tabPage32;
	private: System::Windows::Forms::TextBox^  textBox19;
	private: System::Windows::Forms::Label^  label94;
	private: System::Windows::Forms::TabPage^  tabPage33;
	private: System::Windows::Forms::TextBox^  textBox20;
	private: System::Windows::Forms::Label^  label95;
	private: System::Windows::Forms::TextBox^  textBox21;
	private: System::Windows::Forms::Label^  label96;
	private: System::Windows::Forms::TextBox^  textBox22;
	private: System::Windows::Forms::Label^  label97;
	private: System::Windows::Forms::TabPage^  tabPage34;
	private: System::Windows::Forms::TextBox^  textBox23;
	private: System::Windows::Forms::Label^  label98;
	private: System::Windows::Forms::TextBox^  textBox24;
	private: System::Windows::Forms::Label^  label99;
	private: System::Windows::Forms::TabPage^  tabPage35;
	private: System::Windows::Forms::TextBox^  textBox25;
	private: System::Windows::Forms::Label^  label100;
	private: System::Windows::Forms::TextBox^  textBox26;
	private: System::Windows::Forms::Label^  label101;
	private: System::Windows::Forms::TabPage^  tabPage36;
	private: System::Windows::Forms::TextBox^  textBox27;
	private: System::Windows::Forms::Label^  label102;
	private: System::Windows::Forms::TextBox^  textBox28;
	private: System::Windows::Forms::Label^  label103;
	private: System::Windows::Forms::TextBox^  textBox29;
	private: System::Windows::Forms::Label^  label104;
	private: System::Windows::Forms::GroupBox^  groupBox13;
	private: System::Windows::Forms::Button^  button30;
	private: System::Windows::Forms::Label^  label105;
	private: System::Windows::Forms::TextBox^  textBox30;
	private: System::Windows::Forms::TextBox^  textBox31;
	private: System::Windows::Forms::Label^  label106;
	private: System::Windows::Forms::Button^  button31;
	private: System::Windows::Forms::TabPage^  tabPage37;
	private: System::Windows::Forms::TabControl^  tabControl10;
	private: System::Windows::Forms::TabPage^  tabPage38;
	private: System::Windows::Forms::Button^  button32;
	private: System::Windows::Forms::TextBox^  textBox32;
	private: System::Windows::Forms::Label^  label107;
	private: System::Windows::Forms::Button^  button33;
	private: System::Windows::Forms::TextBox^  textBox33;
	private: System::Windows::Forms::Label^  label108;
	private: System::Windows::Forms::Button^  button34;
	private: System::Windows::Forms::Button^  button35;
	private: System::Windows::Forms::TextBox^  textBox34;
	private: System::Windows::Forms::Label^  label109;
	private: System::Windows::Forms::Button^  button36;
	private: System::Windows::Forms::TextBox^  textBox35;
	private: System::Windows::Forms::Label^  label110;
	private: System::Windows::Forms::Button^  button37;
	private: System::Windows::Forms::TextBox^  textBox36;
	private: System::Windows::Forms::Label^  label111;
	private: System::Windows::Forms::Button^  button38;
	private: System::Windows::Forms::TextBox^  textBox37;
	private: System::Windows::Forms::Label^  label112;
	private: System::Windows::Forms::Button^  button39;
	private: System::Windows::Forms::TextBox^  textBox38;
	private: System::Windows::Forms::Label^  label113;
	private: System::Windows::Forms::Button^  button40;
	private: System::Windows::Forms::TextBox^  textBox39;
	private: System::Windows::Forms::Label^  label114;
	private: System::Windows::Forms::Button^  button41;
	private: System::Windows::Forms::TextBox^  textBox40;
	private: System::Windows::Forms::Label^  label115;
	private: System::Windows::Forms::TabPage^  tabPage39;
	private: System::Windows::Forms::Button^  button42;
	private: System::Windows::Forms::GroupBox^  groupBox14;
	private: System::Windows::Forms::CheckBox^  checkBox7;
	private: System::Windows::Forms::CheckBox^  checkBox8;
	private: System::Windows::Forms::CheckBox^  checkBox9;
	private: System::Windows::Forms::CheckBox^  checkBox10;
	private: System::Windows::Forms::CheckBox^  checkBox11;
	private: System::Windows::Forms::CheckBox^  checkBox12;
	private: System::Windows::Forms::TabControl^  tabControl11;
	private: System::Windows::Forms::TabPage^  tabPage40;
	private: System::Windows::Forms::TextBox^  textBox41;
	private: System::Windows::Forms::Label^  label116;
	private: System::Windows::Forms::TabPage^  tabPage41;
	private: System::Windows::Forms::TextBox^  textBox42;
	private: System::Windows::Forms::Label^  label117;
	private: System::Windows::Forms::TextBox^  textBox43;
	private: System::Windows::Forms::Label^  label118;
	private: System::Windows::Forms::TextBox^  textBox44;
	private: System::Windows::Forms::Label^  label119;
	private: System::Windows::Forms::TabPage^  tabPage42;
	private: System::Windows::Forms::TextBox^  textBox45;
	private: System::Windows::Forms::Label^  label120;
	private: System::Windows::Forms::TextBox^  textBox46;
	private: System::Windows::Forms::Label^  label121;
	private: System::Windows::Forms::TabPage^  tabPage43;
	private: System::Windows::Forms::TextBox^  textBox47;
	private: System::Windows::Forms::Label^  label122;
	private: System::Windows::Forms::TextBox^  textBox48;
	private: System::Windows::Forms::Label^  label123;
	private: System::Windows::Forms::TabPage^  tabPage44;
	private: System::Windows::Forms::TextBox^  textBox49;
	private: System::Windows::Forms::Label^  label124;
	private: System::Windows::Forms::TextBox^  textBox50;
	private: System::Windows::Forms::Label^  label125;
	private: System::Windows::Forms::TextBox^  textBox51;
	private: System::Windows::Forms::Label^  label126;
	private: System::Windows::Forms::GroupBox^  groupBox15;
	private: System::Windows::Forms::Button^  button43;
	private: System::Windows::Forms::Label^  label127;
	private: System::Windows::Forms::TextBox^  textBox52;
	private: System::Windows::Forms::TextBox^  textBox53;
	private: System::Windows::Forms::Label^  label128;
	private: System::Windows::Forms::Button^  button44;
	private: System::Windows::Forms::TabPage^  tabPage45;
	private: System::Windows::Forms::TextBox^  textBox54;
	private: System::Windows::Forms::Label^  label129;
	private: System::Windows::Forms::TextBox^  textBox55;
	private: System::Windows::Forms::Label^  label130;
	private: System::Windows::Forms::Button^  button45;
	private: System::Windows::Forms::TabControl^  tabControl12;
	private: System::Windows::Forms::TabPage^  tabPage46;
	private: System::Windows::Forms::Button^  button46;
	private: System::Windows::Forms::TextBox^  textBox56;
	private: System::Windows::Forms::Label^  label131;
	private: System::Windows::Forms::Button^  button47;
	private: System::Windows::Forms::TextBox^  textBox57;
	private: System::Windows::Forms::Label^  label132;
	private: System::Windows::Forms::Button^  button48;
	private: System::Windows::Forms::TextBox^  textBox58;
	private: System::Windows::Forms::Label^  label133;
	private: System::Windows::Forms::Button^  button49;
	private: System::Windows::Forms::TextBox^  textBox59;
	private: System::Windows::Forms::Label^  label134;
	private: System::Windows::Forms::Button^  button50;
	private: System::Windows::Forms::Button^  button51;
	private: System::Windows::Forms::TextBox^  textBox60;
	private: System::Windows::Forms::Label^  label135;
	private: System::Windows::Forms::Button^  button52;
	private: System::Windows::Forms::TextBox^  textBox61;
	private: System::Windows::Forms::Label^  label136;
	private: System::Windows::Forms::Button^  button53;
	private: System::Windows::Forms::TextBox^  textBox62;
	private: System::Windows::Forms::Label^  label137;
	private: System::Windows::Forms::Button^  button54;
	private: System::Windows::Forms::TextBox^  textBox63;
	private: System::Windows::Forms::Label^  label138;
	private: System::Windows::Forms::TabPage^  tabPage47;
	private: System::Windows::Forms::Button^  button55;
	private: System::Windows::Forms::GroupBox^  groupBox16;
	private: System::Windows::Forms::CheckBox^  checkBox13;
	private: System::Windows::Forms::CheckBox^  checkBox14;
	private: System::Windows::Forms::CheckBox^  checkBox15;
	private: System::Windows::Forms::CheckBox^  checkBox16;
	private: System::Windows::Forms::CheckBox^  checkBox17;
	private: System::Windows::Forms::CheckBox^  checkBox18;
	private: System::Windows::Forms::TabControl^  tabControl13;
	private: System::Windows::Forms::TabPage^  tabPage48;
	private: System::Windows::Forms::TextBox^  textBox64;
	private: System::Windows::Forms::Label^  label139;
	private: System::Windows::Forms::TabPage^  tabPage49;
	private: System::Windows::Forms::TextBox^  textBox65;
	private: System::Windows::Forms::Label^  label140;
	private: System::Windows::Forms::TextBox^  textBox66;
	private: System::Windows::Forms::Label^  label141;
	private: System::Windows::Forms::TextBox^  textBox67;
	private: System::Windows::Forms::Label^  label142;
	private: System::Windows::Forms::TabPage^  tabPage50;
	private: System::Windows::Forms::TextBox^  textBox68;
	private: System::Windows::Forms::Label^  label143;
	private: System::Windows::Forms::TextBox^  textBox69;
	private: System::Windows::Forms::Label^  label144;
	private: System::Windows::Forms::TabPage^  tabPage51;
	private: System::Windows::Forms::TextBox^  textBox70;
	private: System::Windows::Forms::Label^  label145;
	private: System::Windows::Forms::TextBox^  textBox71;
	private: System::Windows::Forms::Label^  label146;
	private: System::Windows::Forms::TabPage^  tabPage52;
	private: System::Windows::Forms::TextBox^  textBox72;
	private: System::Windows::Forms::Label^  label147;
	private: System::Windows::Forms::TextBox^  textBox73;
	private: System::Windows::Forms::Label^  label148;
	private: System::Windows::Forms::TabPage^  tabPage53;
	private: System::Windows::Forms::Button^  button56;
	private: System::Windows::Forms::Button^  button57;
	private: System::Windows::Forms::Label^  label149;
	private: System::Windows::Forms::TextBox^  textBox74;
	private: System::Windows::Forms::TextBox^  textBox75;
	private: System::Windows::Forms::GroupBox^  groupBox17;
	private: System::Windows::Forms::Button^  button58;
	private: System::Windows::Forms::Label^  label150;
	private: System::Windows::Forms::TextBox^  textBox76;
	private: System::Windows::Forms::TabPage^  tabPage54;
	private: System::Windows::Forms::Button^  button59;
	private: System::Windows::Forms::Label^  label151;
	private: System::Windows::Forms::TextBox^  textBox77;
	private: System::Windows::Forms::Button^  button60;
	private: System::Windows::Forms::TextBox^  textBox78;
	private: System::Windows::Forms::CheckBox^  checkBox19;
	private: System::Windows::Forms::TabPage^  tabPage55;
	private: System::Windows::Forms::GroupBox^  groupBox18;
	private: System::Windows::Forms::Button^  button61;
	private: System::Windows::Forms::TextBox^  textBox79;
	private: System::Windows::Forms::Label^  label152;
	private: System::Windows::Forms::Button^  button62;
	private: System::Windows::Forms::Button^  button63;
	private: System::Windows::Forms::TextBox^  textBox80;
	private: System::Windows::Forms::Button^  button64;
	private: System::Windows::Forms::TextBox^  textBox81;
	private: System::Windows::Forms::Label^  label153;
	private: System::Windows::Forms::TextBox^  textBox82;
	private: System::Windows::Forms::Label^  label154;
	private: System::Windows::Forms::Button^  button65;
	private: System::Windows::Forms::Label^  label155;
	private: System::Windows::Forms::TextBox^  textBox83;
	private: System::Windows::Forms::Button^  button66;
	private: System::Windows::Forms::Label^  label156;
	private: System::Windows::Forms::TextBox^  textBox84;
	private: System::Windows::Forms::Label^  label157;
	private: System::Windows::Forms::Button^  button67;
	private: System::Windows::Forms::Label^  label158;
	private: System::Windows::Forms::Button^  button68;
	private: System::Windows::Forms::Button^  button69;
	private: System::Windows::Forms::TextBox^  textBox85;
	private: System::Windows::Forms::TextBox^  textBox86;
	private: System::Windows::Forms::Button^  button70;
	private: System::Windows::Forms::TextBox^  textBox87;
	private: System::Windows::Forms::Button^  button71;
	private: System::Windows::Forms::TextBox^  textBox88;
	private: System::Windows::Forms::Label^  label159;
	private: System::Windows::Forms::Label^  label160;
	private: System::Windows::Forms::Label^  label161;
	private: System::Windows::Forms::Button^  button72;
	private: System::Windows::Forms::TextBox^  textBox89;
	private: System::Windows::Forms::Button^  button73;
	private: System::Windows::Forms::TextBox^  textBox90;
	private: System::Windows::Forms::Button^  button74;
	private: System::Windows::Forms::TextBox^  textBox91;
	private: System::Windows::Forms::Label^  label162;
	private: System::Windows::Forms::Label^  label163;
	private: System::Windows::Forms::Label^  label164;
	private: System::Windows::Forms::GroupBox^  groupBox19;
	private: System::Windows::Forms::Button^  button75;
	private: System::Windows::Forms::Label^  label165;
	private: System::Windows::Forms::TextBox^  textBox92;
	private: System::Windows::Forms::TabPage^  tabPage56;
	private: System::Windows::Forms::TabControl^  tabControl14;
	private: System::Windows::Forms::TabPage^  tabPage57;
	private: System::Windows::Forms::Button^  button76;
	private: System::Windows::Forms::Button^  button77;
	private: System::Windows::Forms::TextBox^  textBox93;
	private: System::Windows::Forms::Label^  label166;
	private: System::Windows::Forms::Button^  button78;
	private: System::Windows::Forms::TextBox^  textBox94;
	private: System::Windows::Forms::Label^  label167;
	private: System::Windows::Forms::TextBox^  textBox95;
	private: System::Windows::Forms::Label^  label168;
	private: System::Windows::Forms::Button^  button79;
	private: System::Windows::Forms::TabPage^  tabPage58;
	private: System::Windows::Forms::Button^  button80;
	private: System::Windows::Forms::GroupBox^  groupBox20;
	private: System::Windows::Forms::CheckBox^  checkBox20;
	private: System::Windows::Forms::CheckBox^  checkBox21;
	private: System::Windows::Forms::CheckBox^  checkBox22;
	private: System::Windows::Forms::CheckBox^  checkBox23;
	private: System::Windows::Forms::CheckBox^  checkBox24;
	private: System::Windows::Forms::CheckBox^  checkBox25;
	private: System::Windows::Forms::TabControl^  tabControl15;
	private: System::Windows::Forms::TabPage^  tabPage59;
	private: System::Windows::Forms::TextBox^  textBox96;
	private: System::Windows::Forms::Label^  label169;
	private: System::Windows::Forms::TabPage^  tabPage60;
	private: System::Windows::Forms::TextBox^  textBox97;
	private: System::Windows::Forms::Label^  label170;
	private: System::Windows::Forms::TextBox^  textBox98;
	private: System::Windows::Forms::Label^  label171;
	private: System::Windows::Forms::TextBox^  textBox99;
	private: System::Windows::Forms::Label^  label172;
	private: System::Windows::Forms::TabPage^  tabPage61;
	private: System::Windows::Forms::TextBox^  textBox100;
	private: System::Windows::Forms::Label^  label173;
	private: System::Windows::Forms::TextBox^  textBox101;
	private: System::Windows::Forms::Label^  label174;
	private: System::Windows::Forms::TabPage^  tabPage62;
	private: System::Windows::Forms::TextBox^  textBox102;
	private: System::Windows::Forms::Label^  label175;
	private: System::Windows::Forms::TextBox^  textBox103;
	private: System::Windows::Forms::Label^  label176;
	private: System::Windows::Forms::TabPage^  tabPage63;
	private: System::Windows::Forms::TextBox^  textBox104;
	private: System::Windows::Forms::Label^  label177;
	private: System::Windows::Forms::TextBox^  textBox105;
	private: System::Windows::Forms::Label^  label178;
	private: System::Windows::Forms::TextBox^  textBox106;
	private: System::Windows::Forms::Label^  label179;
	private: System::Windows::Forms::GroupBox^  groupBox21;
	private: System::Windows::Forms::Button^  button81;
	private: System::Windows::Forms::Label^  label180;
	private: System::Windows::Forms::TextBox^  textBox107;
	private: System::Windows::Forms::TextBox^  textBox108;
	private: System::Windows::Forms::Label^  label181;
	private: System::Windows::Forms::Button^  button82;
	private: System::Windows::Forms::TabPage^  tabPage64;
	private: System::Windows::Forms::TabControl^  tabControl16;
	private: System::Windows::Forms::TabPage^  tabPage65;
	private: System::Windows::Forms::Button^  button83;
	private: System::Windows::Forms::TextBox^  textBox109;
	private: System::Windows::Forms::Label^  label182;
	private: System::Windows::Forms::Button^  button84;
	private: System::Windows::Forms::TextBox^  textBox110;
	private: System::Windows::Forms::Label^  label183;
	private: System::Windows::Forms::Button^  button85;
	private: System::Windows::Forms::Button^  button86;
	private: System::Windows::Forms::TextBox^  textBox111;
	private: System::Windows::Forms::Label^  label184;
	private: System::Windows::Forms::Button^  button87;
	private: System::Windows::Forms::TextBox^  textBox112;
	private: System::Windows::Forms::Label^  label185;
	private: System::Windows::Forms::Button^  button88;
	private: System::Windows::Forms::TextBox^  textBox113;
	private: System::Windows::Forms::Label^  label186;
	private: System::Windows::Forms::Button^  button89;
	private: System::Windows::Forms::TextBox^  textBox114;
	private: System::Windows::Forms::Label^  label187;
	private: System::Windows::Forms::Button^  button90;
	private: System::Windows::Forms::TextBox^  textBox115;
	private: System::Windows::Forms::Label^  label188;
	private: System::Windows::Forms::Button^  button91;
	private: System::Windows::Forms::TextBox^  textBox116;
	private: System::Windows::Forms::Label^  label189;
	private: System::Windows::Forms::Button^  button92;
	private: System::Windows::Forms::TextBox^  textBox117;
	private: System::Windows::Forms::Label^  label190;
	private: System::Windows::Forms::TabPage^  tabPage66;
	private: System::Windows::Forms::Button^  button93;
	private: System::Windows::Forms::GroupBox^  groupBox22;
	private: System::Windows::Forms::CheckBox^  checkBox26;
	private: System::Windows::Forms::CheckBox^  checkBox27;
	private: System::Windows::Forms::CheckBox^  checkBox28;
	private: System::Windows::Forms::CheckBox^  checkBox29;
	private: System::Windows::Forms::CheckBox^  checkBox30;
	private: System::Windows::Forms::CheckBox^  checkBox31;
	private: System::Windows::Forms::TabControl^  tabControl17;
	private: System::Windows::Forms::TabPage^  tabPage67;
	private: System::Windows::Forms::TextBox^  textBox118;
	private: System::Windows::Forms::Label^  label191;
	private: System::Windows::Forms::TabPage^  tabPage68;
	private: System::Windows::Forms::TextBox^  textBox119;
	private: System::Windows::Forms::Label^  label192;
	private: System::Windows::Forms::TextBox^  textBox120;
	private: System::Windows::Forms::Label^  label193;
	private: System::Windows::Forms::TextBox^  textBox121;
	private: System::Windows::Forms::Label^  label194;
	private: System::Windows::Forms::TabPage^  tabPage69;
	private: System::Windows::Forms::TextBox^  textBox122;
	private: System::Windows::Forms::Label^  label195;
	private: System::Windows::Forms::TextBox^  textBox123;
	private: System::Windows::Forms::Label^  label196;
	private: System::Windows::Forms::TabPage^  tabPage70;
	private: System::Windows::Forms::TextBox^  textBox124;
	private: System::Windows::Forms::Label^  label197;
	private: System::Windows::Forms::TextBox^  textBox125;
	private: System::Windows::Forms::Label^  label198;
	private: System::Windows::Forms::TabPage^  tabPage71;
	private: System::Windows::Forms::TextBox^  textBox126;
	private: System::Windows::Forms::Label^  label199;
	private: System::Windows::Forms::TextBox^  textBox127;
	private: System::Windows::Forms::Label^  label200;
	private: System::Windows::Forms::TextBox^  textBox128;
	private: System::Windows::Forms::Label^  label201;
	private: System::Windows::Forms::GroupBox^  groupBox23;
	private: System::Windows::Forms::Button^  button94;
	private: System::Windows::Forms::Label^  label202;
	private: System::Windows::Forms::TextBox^  textBox129;
	private: System::Windows::Forms::TextBox^  textBox130;
	private: System::Windows::Forms::Label^  label203;
	private: System::Windows::Forms::Button^  button95;
	private: System::Windows::Forms::TabPage^  tabPage72;
	private: System::Windows::Forms::TextBox^  textBox131;
	private: System::Windows::Forms::Label^  label204;
	private: System::Windows::Forms::TextBox^  textBox132;
	private: System::Windows::Forms::Label^  label205;
	private: System::Windows::Forms::Button^  button96;
	private: System::Windows::Forms::TabControl^  tabControl18;
	private: System::Windows::Forms::TabPage^  tabPage73;
	private: System::Windows::Forms::Button^  button97;
	private: System::Windows::Forms::TextBox^  textBox133;
	private: System::Windows::Forms::Label^  label206;
	private: System::Windows::Forms::Button^  button98;
	private: System::Windows::Forms::TextBox^  textBox134;
	private: System::Windows::Forms::Label^  label207;
	private: System::Windows::Forms::Button^  button99;
	private: System::Windows::Forms::TextBox^  textBox135;
	private: System::Windows::Forms::Label^  label208;
	private: System::Windows::Forms::Button^  button100;
	private: System::Windows::Forms::TextBox^  textBox136;
	private: System::Windows::Forms::Label^  label209;
	private: System::Windows::Forms::Button^  button101;
	private: System::Windows::Forms::Button^  button102;
	private: System::Windows::Forms::TextBox^  textBox137;
	private: System::Windows::Forms::Label^  label210;
	private: System::Windows::Forms::Button^  button103;
	private: System::Windows::Forms::TextBox^  textBox138;
	private: System::Windows::Forms::Label^  label211;
	private: System::Windows::Forms::Button^  button104;
	private: System::Windows::Forms::TextBox^  textBox139;
	private: System::Windows::Forms::Label^  label212;
	private: System::Windows::Forms::Button^  button105;
	private: System::Windows::Forms::TextBox^  textBox140;
	private: System::Windows::Forms::Label^  label213;
	private: System::Windows::Forms::TabPage^  tabPage74;
	private: System::Windows::Forms::Button^  button106;
	private: System::Windows::Forms::GroupBox^  groupBox24;
	private: System::Windows::Forms::CheckBox^  checkBox32;
	private: System::Windows::Forms::CheckBox^  checkBox33;
	private: System::Windows::Forms::CheckBox^  checkBox34;
	private: System::Windows::Forms::CheckBox^  checkBox35;
	private: System::Windows::Forms::CheckBox^  checkBox36;
	private: System::Windows::Forms::CheckBox^  checkBox37;
	private: System::Windows::Forms::TabControl^  tabControl19;
	private: System::Windows::Forms::TabPage^  tabPage75;
	private: System::Windows::Forms::TextBox^  textBox141;
	private: System::Windows::Forms::Label^  label214;
	private: System::Windows::Forms::TabPage^  tabPage76;
	private: System::Windows::Forms::TextBox^  textBox142;
	private: System::Windows::Forms::Label^  label215;
	private: System::Windows::Forms::TextBox^  textBox143;
	private: System::Windows::Forms::Label^  label216;
	private: System::Windows::Forms::TextBox^  textBox144;
	private: System::Windows::Forms::Label^  label217;
	private: System::Windows::Forms::TabPage^  tabPage77;
	private: System::Windows::Forms::TextBox^  textBox145;
	private: System::Windows::Forms::Label^  label218;
	private: System::Windows::Forms::TextBox^  textBox146;
	private: System::Windows::Forms::Label^  label219;
	private: System::Windows::Forms::TabPage^  tabPage78;
	private: System::Windows::Forms::TextBox^  textBox147;
	private: System::Windows::Forms::Label^  label220;
	private: System::Windows::Forms::TextBox^  textBox148;
	private: System::Windows::Forms::Label^  label221;
	private: System::Windows::Forms::TabPage^  tabPage79;
	private: System::Windows::Forms::TextBox^  textBox149;
	private: System::Windows::Forms::Label^  label222;
	private: System::Windows::Forms::TextBox^  textBox150;
	private: System::Windows::Forms::Label^  label223;
	private: System::Windows::Forms::TabPage^  tabPage80;
	private: System::Windows::Forms::Button^  button107;
	private: System::Windows::Forms::Button^  button108;
	private: System::Windows::Forms::Label^  label224;
	private: System::Windows::Forms::TextBox^  textBox151;
	private: System::Windows::Forms::TextBox^  textBox152;
	private: System::Windows::Forms::GroupBox^  groupBox25;
	private: System::Windows::Forms::Button^  button109;
	private: System::Windows::Forms::Label^  label225;
	private: System::Windows::Forms::TextBox^  textBox153;
	private: System::Windows::Forms::TabPage^  tabPage81;
	private: System::Windows::Forms::Button^  button110;
	private: System::Windows::Forms::Label^  label226;
	private: System::Windows::Forms::TextBox^  textBox154;
	private: System::Windows::Forms::Button^  button111;
	private: System::Windows::Forms::TextBox^  textBox155;
	private: System::Windows::Forms::CheckBox^  checkBox38;
	private: System::Windows::Forms::TabPage^  tabPage82;
	private: System::Windows::Forms::TabPage^  tabPage19;
	private: System::Windows::Forms::Button^  zmienAlfa;
	private: System::Windows::Forms::Label^  label54;
	private: System::Windows::Forms::TextBox^  nowaAlfa;
	private: System::Windows::Forms::TextBox^  identyfikacja_textBox;
	private: System::Windows::Forms::Button^  czyscIdent;
	private: System::Windows::Forms::CheckBox^  identyfikuj;
	private: System::Windows::Forms::TabPage^  tabPage18;
	private: System::Windows::Forms::TextBox^  nazwaPlikuGPC;
	private: System::Windows::Forms::TextBox^  nazwaReg_GPC;
	private: System::Windows::Forms::Label^  label75;
	private: System::Windows::Forms::Label^  label76;
	private: System::Windows::Forms::Button^  zapiszGPC;
	private: System::Windows::Forms::TabControl^  tabControl5;
	private: System::Windows::Forms::TabPage^  tabPage20;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::TextBox^  dBGPC;
	private: System::Windows::Forms::Label^  label59;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::TextBox^  dAGPC;
	private: System::Windows::Forms::Label^  label60;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::TextBox^  wyMinGPC;
	private: System::Windows::Forms::Label^  label56;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::TextBox^  wyMaxGPC;
	private: System::Windows::Forms::Label^  label57;
	private: System::Windows::Forms::Button^  zmienWszystkoGPC;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::TextBox^  alfaGPC;
	private: System::Windows::Forms::Label^  label61;
	private: System::Windows::Forms::Button^  button9;
	private: System::Windows::Forms::TextBox^  roGPC;
	private: System::Windows::Forms::Label^  label62;
	private: System::Windows::Forms::Button^  button10;
	private: System::Windows::Forms::TextBox^  LGPC;
	private: System::Windows::Forms::Label^  label63;
	private: System::Windows::Forms::Button^  button11;
	private: System::Windows::Forms::TextBox^  HGPC;
	private: System::Windows::Forms::Label^  label64;
	private: System::Windows::Forms::TabPage^  tabPage21;
	private: System::Windows::Forms::Button^  zmienWymuszenieGPC;
	private: System::Windows::Forms::GroupBox^  groupBox8;
	private: System::Windows::Forms::CheckBox^  skokGPC;
	private: System::Windows::Forms::CheckBox^  szumGPC;
	private: System::Windows::Forms::CheckBox^  kronekerGPC;
	private: System::Windows::Forms::CheckBox^  trojkatGPC;
	private: System::Windows::Forms::CheckBox^  prostokatGPC;
	private: System::Windows::Forms::CheckBox^  sinusGPC;
	private: System::Windows::Forms::TabControl^  tabControl6;
	private: System::Windows::Forms::TabPage^  tabPage22;
	private: System::Windows::Forms::TextBox^  nowySkokGPC;
	private: System::Windows::Forms::Label^  label65;
	private: System::Windows::Forms::TabPage^  tabPage23;
	private: System::Windows::Forms::TextBox^  noweWypelnieniePGPC;
	private: System::Windows::Forms::Label^  label66;
	private: System::Windows::Forms::TextBox^  nowyOkresPGPC;
	private: System::Windows::Forms::Label^  label67;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaPGPC;
	private: System::Windows::Forms::Label^  label68;
	private: System::Windows::Forms::TabPage^  tabPage24;
	private: System::Windows::Forms::TextBox^  nowyOkresSGPC;
	private: System::Windows::Forms::Label^  label69;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaSGPC;
	private: System::Windows::Forms::Label^  label70;
	private: System::Windows::Forms::TabPage^  tabPage25;
	private: System::Windows::Forms::TextBox^  nowyOkresTGPC;
	private: System::Windows::Forms::Label^  label71;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaTGPC;
	private: System::Windows::Forms::Label^  label72;
	private: System::Windows::Forms::TabPage^  tabPage26;
	private: System::Windows::Forms::TextBox^  nowaLiczbaProbekGPC;
	private: System::Windows::Forms::Label^  label73;
	private: System::Windows::Forms::TextBox^  nowaWariancjaGPC;
	private: System::Windows::Forms::Label^  label74;
	private: System::Windows::Forms::TabPage^  tabPage27;
	private: System::Windows::Forms::Button^  czyscIdentGPC;
	private: System::Windows::Forms::Button^  zmiennAlfaGPC;
	private: System::Windows::Forms::Label^  label58;
	private: System::Windows::Forms::TextBox^  identyfikacjaGPC_textBox;
	private: System::Windows::Forms::TextBox^  nowaAlfaGPC;
	private: System::Windows::Forms::GroupBox^  groupBox4;
	private: System::Windows::Forms::Button^  wczytajRegGPC;
	private: System::Windows::Forms::Label^  label55;
	private: System::Windows::Forms::TextBox^  nazwaRegGPC;
	private: System::Windows::Forms::TabPage^  tabPage8;
	private: System::Windows::Forms::TabControl^  tabControl3;
	private: System::Windows::Forms::TabPage^  tabPage14;
	private: System::Windows::Forms::Button^  zmienWyMinPID;
	private: System::Windows::Forms::TextBox^  wyMinPID;
	private: System::Windows::Forms::Label^  label50;
	private: System::Windows::Forms::Button^  zmienWyMaxPID;
	private: System::Windows::Forms::TextBox^  wyMaxPID;
	private: System::Windows::Forms::Label^  label51;
	private: System::Windows::Forms::Button^  zmienWszystkoPID;
	private: System::Windows::Forms::Button^  zmienBPID;
	private: System::Windows::Forms::TextBox^  bPID;
	private: System::Windows::Forms::Label^  label49;
	private: System::Windows::Forms::Button^  zmienNPID;
	private: System::Windows::Forms::TextBox^  nPID;
	private: System::Windows::Forms::Label^  label48;
	private: System::Windows::Forms::Button^  zmienWpPID;
	private: System::Windows::Forms::TextBox^  wpPID;
	private: System::Windows::Forms::Label^  label47;
	private: System::Windows::Forms::Button^  zmienTpPID;
	private: System::Windows::Forms::TextBox^  tpPID;
	private: System::Windows::Forms::Label^  label46;
	private: System::Windows::Forms::Button^  zmienTdPID;
	private: System::Windows::Forms::TextBox^  tdPID;
	private: System::Windows::Forms::Label^  label45;
	private: System::Windows::Forms::Button^  zmienTiPID;
	private: System::Windows::Forms::TextBox^  tiPID;
	private: System::Windows::Forms::Label^  label44;
	private: System::Windows::Forms::Button^  zmienKPID;
	private: System::Windows::Forms::TextBox^  kPID;
	private: System::Windows::Forms::Label^  label43;
	private: System::Windows::Forms::TabPage^  tabPage15;
	private: System::Windows::Forms::Button^  zmienWymuszeniePID;
	private: System::Windows::Forms::GroupBox^  groupBox9;
	private: System::Windows::Forms::CheckBox^  skokPID;
	private: System::Windows::Forms::CheckBox^  szumPID;
	private: System::Windows::Forms::CheckBox^  kronekerPID;
	private: System::Windows::Forms::CheckBox^  trojkatPID;
	private: System::Windows::Forms::CheckBox^  prostokatPID;
	private: System::Windows::Forms::CheckBox^  sinusPID;
	private: System::Windows::Forms::TabControl^  tabControl2;
	private: System::Windows::Forms::TabPage^  tabPage9;
	private: System::Windows::Forms::TextBox^  nowySkokPID;
	private: System::Windows::Forms::Label^  label31;
	private: System::Windows::Forms::TabPage^  tabPage10;
	private: System::Windows::Forms::TextBox^  noweWypelnieniePPID;
	private: System::Windows::Forms::Label^  label32;
	private: System::Windows::Forms::TextBox^  nowyOkresPPID;
	private: System::Windows::Forms::Label^  label33;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaPPID;
	private: System::Windows::Forms::Label^  label34;
	private: System::Windows::Forms::TabPage^  tabPage11;
	private: System::Windows::Forms::TextBox^  nowyOkresSPID;
	private: System::Windows::Forms::Label^  label35;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaSPID;
	private: System::Windows::Forms::Label^  label36;
	private: System::Windows::Forms::TabPage^  tabPage12;
	private: System::Windows::Forms::TextBox^  nowyOkresTPID;
	private: System::Windows::Forms::Label^  label37;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaTPID;
	private: System::Windows::Forms::Label^  label38;
	private: System::Windows::Forms::TabPage^  tabPage13;
	private: System::Windows::Forms::TextBox^  nowaLiczbaProbekPID;
	private: System::Windows::Forms::Label^  label39;
	private: System::Windows::Forms::TextBox^  nowaWariancjaPID;
	private: System::Windows::Forms::Label^  label40;
	private: System::Windows::Forms::TextBox^  nazwaPlikuPID;
	private: System::Windows::Forms::TextBox^  nazwaReg_PID;
	private: System::Windows::Forms::Label^  label41;
	private: System::Windows::Forms::GroupBox^  groupBox7;
	private: System::Windows::Forms::Button^  wczytajRegPID;
	private: System::Windows::Forms::Label^  label30;
	private: System::Windows::Forms::TextBox^  nazwaRegPID;
	private: System::Windows::Forms::Label^  label42;
	private: System::Windows::Forms::Button^  zapiszPID;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabControl^  tabControl4;
	private: System::Windows::Forms::TabPage^  tabPage16;
	private: System::Windows::Forms::Button^  zmienWszystkoP;
	private: System::Windows::Forms::Button^  zmienWyMinP;
	private: System::Windows::Forms::TextBox^  wyMinP;
	private: System::Windows::Forms::Label^  label52;
	private: System::Windows::Forms::Button^  zmienWyMaxP;
	private: System::Windows::Forms::TextBox^  wyMaxP;
	private: System::Windows::Forms::Label^  label53;
	private: System::Windows::Forms::TextBox^  k;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Button^  zmienK;
	private: System::Windows::Forms::TabPage^  tabPage17;
	private: System::Windows::Forms::Button^  zmienWymuszenieP;
	private: System::Windows::Forms::GroupBox^  groupBox6;
	private: System::Windows::Forms::CheckBox^  skokP;
	private: System::Windows::Forms::CheckBox^  szumP;
	private: System::Windows::Forms::CheckBox^  kronekerP;
	private: System::Windows::Forms::CheckBox^  trojkatP;
	private: System::Windows::Forms::CheckBox^  prostokatP;
	private: System::Windows::Forms::CheckBox^  sinusP;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::TextBox^  nowySkokP;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::TextBox^  noweWypelnieniePP;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::TextBox^  nowyOkresPP;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaPP;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::TabPage^  tabPage5;
	private: System::Windows::Forms::TextBox^  nowyOkresSP;
	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaSP;
	private: System::Windows::Forms::Label^  label22;
	private: System::Windows::Forms::TabPage^  tabPage6;
	private: System::Windows::Forms::TextBox^  nowyOkresTP;
	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaTP;
	private: System::Windows::Forms::Label^  label24;
	private: System::Windows::Forms::TabPage^  tabPage7;
	private: System::Windows::Forms::TextBox^  nowaLiczbaProbekP;
	private: System::Windows::Forms::Label^  label25;
	private: System::Windows::Forms::TextBox^  nowaWariancjaP;
	private: System::Windows::Forms::Label^  label26;
	private: System::Windows::Forms::TextBox^  nazwaPlikuP;
	private: System::Windows::Forms::TextBox^  nazwaReg_P;
	private: System::Windows::Forms::Label^  label28;
	private: System::Windows::Forms::GroupBox^  groupBox5;
	private: System::Windows::Forms::Button^  wczytajRegP;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::TextBox^  nazwaRegP;
	private: System::Windows::Forms::Label^  label27;
	private: System::Windows::Forms::Button^  zapiszP;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Button^  zmienDlZakl;
	private: System::Windows::Forms::TextBox^  dlZakl;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Button^  zmienWszystko;
	private: System::Windows::Forms::Button^  zmienI2;
	private: System::Windows::Forms::TextBox^  i2;
	private: System::Windows::Forms::Button^  zmienI1;
	private: System::Windows::Forms::TextBox^  wyMin;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  i1;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Button^  zmienD;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::TextBox^  d;
	private: System::Windows::Forms::Button^  zmienWyMin;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::TextBox^  wyMax;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Button^  zmienWariancja;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Button^  zmienWyMax;
	private: System::Windows::Forms::Button^  zmienC2;
	private: System::Windows::Forms::TextBox^  wariancja;
	private: System::Windows::Forms::TextBox^  c2;
	private: System::Windows::Forms::Button^  zmienB2;
	private: System::Windows::Forms::TextBox^  b2;
	private: System::Windows::Forms::Button^  zmienA2;
	private: System::Windows::Forms::TextBox^  a2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Button^  zmienC1;
	private: System::Windows::Forms::TextBox^  c1;
	private: System::Windows::Forms::Button^  zmienB1;
	private: System::Windows::Forms::TextBox^  b1;
	private: System::Windows::Forms::Button^  zmienA1;
	private: System::Windows::Forms::TextBox^  a1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::Button^  wczytaj;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::TextBox^  nazwaObiektu;
	private: System::Windows::Forms::TabControl^  tabControl;
	private: System::Windows::Forms::TabControl^  tabControl7;
	private: System::Windows::Forms::TabPage^  tabPage83;
	private: System::Windows::Forms::Button^  zmienWyMinAPID;
	private: System::Windows::Forms::TextBox^  wyMinAPID;
	private: System::Windows::Forms::Label^  label227;
	private: System::Windows::Forms::Button^  zmienWyMaxAPID;
	private: System::Windows::Forms::TextBox^  wyMaxAPID;
	private: System::Windows::Forms::Label^  label228;
	private: System::Windows::Forms::Button^  zmienWszystkoAPID;
	private: System::Windows::Forms::Button^  zmienOnAPID;
	private: System::Windows::Forms::TextBox^  onAPID;
	private: System::Windows::Forms::Label^  label232;
	private: System::Windows::Forms::Button^  zmienTdAPID;
	private: System::Windows::Forms::TextBox^  tdAPID;
	private: System::Windows::Forms::Label^  label233;
	private: System::Windows::Forms::Button^  zmienTiAPID;
	private: System::Windows::Forms::TextBox^  tiAPID;
	private: System::Windows::Forms::Label^  label234;
	private: System::Windows::Forms::Button^  zmienKAPID;
	private: System::Windows::Forms::TextBox^  kAPID;
	private: System::Windows::Forms::Label^  label235;
	private: System::Windows::Forms::TabPage^  tabPage84;
	private: System::Windows::Forms::Button^  zmienWymuszenieAPID;
	private: System::Windows::Forms::GroupBox^  groupBox26;
	private: System::Windows::Forms::CheckBox^  skokAPID;
	private: System::Windows::Forms::CheckBox^  szumAPID;
	private: System::Windows::Forms::CheckBox^  kronekerAPID;
	private: System::Windows::Forms::CheckBox^  trojkatAPID;
	private: System::Windows::Forms::CheckBox^  prostokatAPID;
	private: System::Windows::Forms::CheckBox^  sinusAPID;
	private: System::Windows::Forms::TabControl^  tabPage999;
	private: System::Windows::Forms::TabPage^  tabPage85;
	private: System::Windows::Forms::TextBox^  nowySkokAPID;
	private: System::Windows::Forms::Label^  label236;
	private: System::Windows::Forms::TabPage^  tabPage86;
	private: System::Windows::Forms::TextBox^  noweWypelnieniePAPID;
	private: System::Windows::Forms::Label^  label237;
	private: System::Windows::Forms::TextBox^  nowyOkresPAPID;
	private: System::Windows::Forms::Label^  label238;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaPAPID;
	private: System::Windows::Forms::Label^  label239;
	private: System::Windows::Forms::TabPage^  tabPage87;
	private: System::Windows::Forms::TextBox^  nowyOkresSAPID;
	private: System::Windows::Forms::Label^  label240;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaSAPID;
	private: System::Windows::Forms::Label^  label241;
	private: System::Windows::Forms::TabPage^  tabPage88;
	private: System::Windows::Forms::TextBox^  nowyOkresTAPID;
	private: System::Windows::Forms::Label^  label242;
	private: System::Windows::Forms::TextBox^  nowaAmplitudaTAPID;
	private: System::Windows::Forms::Label^  label243;
	private: System::Windows::Forms::TabPage^  tabPage89;
	private: System::Windows::Forms::TextBox^  nowaLiczbaProbekAPID;
	private: System::Windows::Forms::Label^  label244;
	private: System::Windows::Forms::TextBox^  nowaWariancjaAPID;
	private: System::Windows::Forms::Label^  label245;
	private: System::Windows::Forms::CheckBox^  ZmienNastawy;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->start = (gcnew System::Windows::Forms::Button());
			this->stop = (gcnew System::Windows::Forms::CheckBox());
			this->reset = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->czasKrokuTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->czasKrokuNumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->typRegulatora = (gcnew System::Windows::Forms::ComboBox());
			this->tabPage28 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox10 = (gcnew System::Windows::Forms::GroupBox());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label77 = (gcnew System::Windows::Forms::Label());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button12 = (gcnew System::Windows::Forms::Button());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->button13 = (gcnew System::Windows::Forms::Button());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->label78 = (gcnew System::Windows::Forms::Label());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->label79 = (gcnew System::Windows::Forms::Label());
			this->button14 = (gcnew System::Windows::Forms::Button());
			this->label80 = (gcnew System::Windows::Forms::Label());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->button15 = (gcnew System::Windows::Forms::Button());
			this->label81 = (gcnew System::Windows::Forms::Label());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->label82 = (gcnew System::Windows::Forms::Label());
			this->button16 = (gcnew System::Windows::Forms::Button());
			this->label83 = (gcnew System::Windows::Forms::Label());
			this->button17 = (gcnew System::Windows::Forms::Button());
			this->button18 = (gcnew System::Windows::Forms::Button());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->button19 = (gcnew System::Windows::Forms::Button());
			this->textBox10 = (gcnew System::Windows::Forms::TextBox());
			this->button20 = (gcnew System::Windows::Forms::Button());
			this->textBox11 = (gcnew System::Windows::Forms::TextBox());
			this->label84 = (gcnew System::Windows::Forms::Label());
			this->label85 = (gcnew System::Windows::Forms::Label());
			this->label86 = (gcnew System::Windows::Forms::Label());
			this->button21 = (gcnew System::Windows::Forms::Button());
			this->textBox12 = (gcnew System::Windows::Forms::TextBox());
			this->button22 = (gcnew System::Windows::Forms::Button());
			this->textBox13 = (gcnew System::Windows::Forms::TextBox());
			this->button23 = (gcnew System::Windows::Forms::Button());
			this->textBox14 = (gcnew System::Windows::Forms::TextBox());
			this->label87 = (gcnew System::Windows::Forms::Label());
			this->label88 = (gcnew System::Windows::Forms::Label());
			this->label89 = (gcnew System::Windows::Forms::Label());
			this->groupBox11 = (gcnew System::Windows::Forms::GroupBox());
			this->button24 = (gcnew System::Windows::Forms::Button());
			this->label90 = (gcnew System::Windows::Forms::Label());
			this->textBox15 = (gcnew System::Windows::Forms::TextBox());
			this->tabPage29 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl8 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage30 = (gcnew System::Windows::Forms::TabPage());
			this->button25 = (gcnew System::Windows::Forms::Button());
			this->button26 = (gcnew System::Windows::Forms::Button());
			this->textBox16 = (gcnew System::Windows::Forms::TextBox());
			this->label91 = (gcnew System::Windows::Forms::Label());
			this->button27 = (gcnew System::Windows::Forms::Button());
			this->textBox17 = (gcnew System::Windows::Forms::TextBox());
			this->label92 = (gcnew System::Windows::Forms::Label());
			this->textBox18 = (gcnew System::Windows::Forms::TextBox());
			this->label93 = (gcnew System::Windows::Forms::Label());
			this->button28 = (gcnew System::Windows::Forms::Button());
			this->tabPage31 = (gcnew System::Windows::Forms::TabPage());
			this->button29 = (gcnew System::Windows::Forms::Button());
			this->groupBox12 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox5 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox6 = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl9 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage32 = (gcnew System::Windows::Forms::TabPage());
			this->textBox19 = (gcnew System::Windows::Forms::TextBox());
			this->label94 = (gcnew System::Windows::Forms::Label());
			this->tabPage33 = (gcnew System::Windows::Forms::TabPage());
			this->textBox20 = (gcnew System::Windows::Forms::TextBox());
			this->label95 = (gcnew System::Windows::Forms::Label());
			this->textBox21 = (gcnew System::Windows::Forms::TextBox());
			this->label96 = (gcnew System::Windows::Forms::Label());
			this->textBox22 = (gcnew System::Windows::Forms::TextBox());
			this->label97 = (gcnew System::Windows::Forms::Label());
			this->tabPage34 = (gcnew System::Windows::Forms::TabPage());
			this->textBox23 = (gcnew System::Windows::Forms::TextBox());
			this->label98 = (gcnew System::Windows::Forms::Label());
			this->textBox24 = (gcnew System::Windows::Forms::TextBox());
			this->label99 = (gcnew System::Windows::Forms::Label());
			this->tabPage35 = (gcnew System::Windows::Forms::TabPage());
			this->textBox25 = (gcnew System::Windows::Forms::TextBox());
			this->label100 = (gcnew System::Windows::Forms::Label());
			this->textBox26 = (gcnew System::Windows::Forms::TextBox());
			this->label101 = (gcnew System::Windows::Forms::Label());
			this->tabPage36 = (gcnew System::Windows::Forms::TabPage());
			this->textBox27 = (gcnew System::Windows::Forms::TextBox());
			this->label102 = (gcnew System::Windows::Forms::Label());
			this->textBox28 = (gcnew System::Windows::Forms::TextBox());
			this->label103 = (gcnew System::Windows::Forms::Label());
			this->textBox29 = (gcnew System::Windows::Forms::TextBox());
			this->label104 = (gcnew System::Windows::Forms::Label());
			this->groupBox13 = (gcnew System::Windows::Forms::GroupBox());
			this->button30 = (gcnew System::Windows::Forms::Button());
			this->label105 = (gcnew System::Windows::Forms::Label());
			this->textBox30 = (gcnew System::Windows::Forms::TextBox());
			this->textBox31 = (gcnew System::Windows::Forms::TextBox());
			this->label106 = (gcnew System::Windows::Forms::Label());
			this->button31 = (gcnew System::Windows::Forms::Button());
			this->tabPage37 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl10 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage38 = (gcnew System::Windows::Forms::TabPage());
			this->button32 = (gcnew System::Windows::Forms::Button());
			this->textBox32 = (gcnew System::Windows::Forms::TextBox());
			this->label107 = (gcnew System::Windows::Forms::Label());
			this->button33 = (gcnew System::Windows::Forms::Button());
			this->textBox33 = (gcnew System::Windows::Forms::TextBox());
			this->label108 = (gcnew System::Windows::Forms::Label());
			this->button34 = (gcnew System::Windows::Forms::Button());
			this->button35 = (gcnew System::Windows::Forms::Button());
			this->textBox34 = (gcnew System::Windows::Forms::TextBox());
			this->label109 = (gcnew System::Windows::Forms::Label());
			this->button36 = (gcnew System::Windows::Forms::Button());
			this->textBox35 = (gcnew System::Windows::Forms::TextBox());
			this->label110 = (gcnew System::Windows::Forms::Label());
			this->button37 = (gcnew System::Windows::Forms::Button());
			this->textBox36 = (gcnew System::Windows::Forms::TextBox());
			this->label111 = (gcnew System::Windows::Forms::Label());
			this->button38 = (gcnew System::Windows::Forms::Button());
			this->textBox37 = (gcnew System::Windows::Forms::TextBox());
			this->label112 = (gcnew System::Windows::Forms::Label());
			this->button39 = (gcnew System::Windows::Forms::Button());
			this->textBox38 = (gcnew System::Windows::Forms::TextBox());
			this->label113 = (gcnew System::Windows::Forms::Label());
			this->button40 = (gcnew System::Windows::Forms::Button());
			this->textBox39 = (gcnew System::Windows::Forms::TextBox());
			this->label114 = (gcnew System::Windows::Forms::Label());
			this->button41 = (gcnew System::Windows::Forms::Button());
			this->textBox40 = (gcnew System::Windows::Forms::TextBox());
			this->label115 = (gcnew System::Windows::Forms::Label());
			this->tabPage39 = (gcnew System::Windows::Forms::TabPage());
			this->button42 = (gcnew System::Windows::Forms::Button());
			this->groupBox14 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox7 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox8 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox9 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox10 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox11 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox12 = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl11 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage40 = (gcnew System::Windows::Forms::TabPage());
			this->textBox41 = (gcnew System::Windows::Forms::TextBox());
			this->label116 = (gcnew System::Windows::Forms::Label());
			this->tabPage41 = (gcnew System::Windows::Forms::TabPage());
			this->textBox42 = (gcnew System::Windows::Forms::TextBox());
			this->label117 = (gcnew System::Windows::Forms::Label());
			this->textBox43 = (gcnew System::Windows::Forms::TextBox());
			this->label118 = (gcnew System::Windows::Forms::Label());
			this->textBox44 = (gcnew System::Windows::Forms::TextBox());
			this->label119 = (gcnew System::Windows::Forms::Label());
			this->tabPage42 = (gcnew System::Windows::Forms::TabPage());
			this->textBox45 = (gcnew System::Windows::Forms::TextBox());
			this->label120 = (gcnew System::Windows::Forms::Label());
			this->textBox46 = (gcnew System::Windows::Forms::TextBox());
			this->label121 = (gcnew System::Windows::Forms::Label());
			this->tabPage43 = (gcnew System::Windows::Forms::TabPage());
			this->textBox47 = (gcnew System::Windows::Forms::TextBox());
			this->label122 = (gcnew System::Windows::Forms::Label());
			this->textBox48 = (gcnew System::Windows::Forms::TextBox());
			this->label123 = (gcnew System::Windows::Forms::Label());
			this->tabPage44 = (gcnew System::Windows::Forms::TabPage());
			this->textBox49 = (gcnew System::Windows::Forms::TextBox());
			this->label124 = (gcnew System::Windows::Forms::Label());
			this->textBox50 = (gcnew System::Windows::Forms::TextBox());
			this->label125 = (gcnew System::Windows::Forms::Label());
			this->textBox51 = (gcnew System::Windows::Forms::TextBox());
			this->label126 = (gcnew System::Windows::Forms::Label());
			this->groupBox15 = (gcnew System::Windows::Forms::GroupBox());
			this->button43 = (gcnew System::Windows::Forms::Button());
			this->label127 = (gcnew System::Windows::Forms::Label());
			this->textBox52 = (gcnew System::Windows::Forms::TextBox());
			this->textBox53 = (gcnew System::Windows::Forms::TextBox());
			this->label128 = (gcnew System::Windows::Forms::Label());
			this->button44 = (gcnew System::Windows::Forms::Button());
			this->tabPage45 = (gcnew System::Windows::Forms::TabPage());
			this->textBox54 = (gcnew System::Windows::Forms::TextBox());
			this->label129 = (gcnew System::Windows::Forms::Label());
			this->textBox55 = (gcnew System::Windows::Forms::TextBox());
			this->label130 = (gcnew System::Windows::Forms::Label());
			this->button45 = (gcnew System::Windows::Forms::Button());
			this->tabControl12 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage46 = (gcnew System::Windows::Forms::TabPage());
			this->button46 = (gcnew System::Windows::Forms::Button());
			this->textBox56 = (gcnew System::Windows::Forms::TextBox());
			this->label131 = (gcnew System::Windows::Forms::Label());
			this->button47 = (gcnew System::Windows::Forms::Button());
			this->textBox57 = (gcnew System::Windows::Forms::TextBox());
			this->label132 = (gcnew System::Windows::Forms::Label());
			this->button48 = (gcnew System::Windows::Forms::Button());
			this->textBox58 = (gcnew System::Windows::Forms::TextBox());
			this->label133 = (gcnew System::Windows::Forms::Label());
			this->button49 = (gcnew System::Windows::Forms::Button());
			this->textBox59 = (gcnew System::Windows::Forms::TextBox());
			this->label134 = (gcnew System::Windows::Forms::Label());
			this->button50 = (gcnew System::Windows::Forms::Button());
			this->button51 = (gcnew System::Windows::Forms::Button());
			this->textBox60 = (gcnew System::Windows::Forms::TextBox());
			this->label135 = (gcnew System::Windows::Forms::Label());
			this->button52 = (gcnew System::Windows::Forms::Button());
			this->textBox61 = (gcnew System::Windows::Forms::TextBox());
			this->label136 = (gcnew System::Windows::Forms::Label());
			this->button53 = (gcnew System::Windows::Forms::Button());
			this->textBox62 = (gcnew System::Windows::Forms::TextBox());
			this->label137 = (gcnew System::Windows::Forms::Label());
			this->button54 = (gcnew System::Windows::Forms::Button());
			this->textBox63 = (gcnew System::Windows::Forms::TextBox());
			this->label138 = (gcnew System::Windows::Forms::Label());
			this->tabPage47 = (gcnew System::Windows::Forms::TabPage());
			this->button55 = (gcnew System::Windows::Forms::Button());
			this->groupBox16 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox13 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox14 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox15 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox16 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox17 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox18 = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl13 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage48 = (gcnew System::Windows::Forms::TabPage());
			this->textBox64 = (gcnew System::Windows::Forms::TextBox());
			this->label139 = (gcnew System::Windows::Forms::Label());
			this->tabPage49 = (gcnew System::Windows::Forms::TabPage());
			this->textBox65 = (gcnew System::Windows::Forms::TextBox());
			this->label140 = (gcnew System::Windows::Forms::Label());
			this->textBox66 = (gcnew System::Windows::Forms::TextBox());
			this->label141 = (gcnew System::Windows::Forms::Label());
			this->textBox67 = (gcnew System::Windows::Forms::TextBox());
			this->label142 = (gcnew System::Windows::Forms::Label());
			this->tabPage50 = (gcnew System::Windows::Forms::TabPage());
			this->textBox68 = (gcnew System::Windows::Forms::TextBox());
			this->label143 = (gcnew System::Windows::Forms::Label());
			this->textBox69 = (gcnew System::Windows::Forms::TextBox());
			this->label144 = (gcnew System::Windows::Forms::Label());
			this->tabPage51 = (gcnew System::Windows::Forms::TabPage());
			this->textBox70 = (gcnew System::Windows::Forms::TextBox());
			this->label145 = (gcnew System::Windows::Forms::Label());
			this->textBox71 = (gcnew System::Windows::Forms::TextBox());
			this->label146 = (gcnew System::Windows::Forms::Label());
			this->tabPage52 = (gcnew System::Windows::Forms::TabPage());
			this->textBox72 = (gcnew System::Windows::Forms::TextBox());
			this->label147 = (gcnew System::Windows::Forms::Label());
			this->textBox73 = (gcnew System::Windows::Forms::TextBox());
			this->label148 = (gcnew System::Windows::Forms::Label());
			this->tabPage53 = (gcnew System::Windows::Forms::TabPage());
			this->button56 = (gcnew System::Windows::Forms::Button());
			this->button57 = (gcnew System::Windows::Forms::Button());
			this->label149 = (gcnew System::Windows::Forms::Label());
			this->textBox74 = (gcnew System::Windows::Forms::TextBox());
			this->textBox75 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox17 = (gcnew System::Windows::Forms::GroupBox());
			this->button58 = (gcnew System::Windows::Forms::Button());
			this->label150 = (gcnew System::Windows::Forms::Label());
			this->textBox76 = (gcnew System::Windows::Forms::TextBox());
			this->tabPage54 = (gcnew System::Windows::Forms::TabPage());
			this->button59 = (gcnew System::Windows::Forms::Button());
			this->label151 = (gcnew System::Windows::Forms::Label());
			this->textBox77 = (gcnew System::Windows::Forms::TextBox());
			this->button60 = (gcnew System::Windows::Forms::Button());
			this->textBox78 = (gcnew System::Windows::Forms::TextBox());
			this->checkBox19 = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage55 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox18 = (gcnew System::Windows::Forms::GroupBox());
			this->button61 = (gcnew System::Windows::Forms::Button());
			this->textBox79 = (gcnew System::Windows::Forms::TextBox());
			this->label152 = (gcnew System::Windows::Forms::Label());
			this->button62 = (gcnew System::Windows::Forms::Button());
			this->button63 = (gcnew System::Windows::Forms::Button());
			this->textBox80 = (gcnew System::Windows::Forms::TextBox());
			this->button64 = (gcnew System::Windows::Forms::Button());
			this->textBox81 = (gcnew System::Windows::Forms::TextBox());
			this->label153 = (gcnew System::Windows::Forms::Label());
			this->textBox82 = (gcnew System::Windows::Forms::TextBox());
			this->label154 = (gcnew System::Windows::Forms::Label());
			this->button65 = (gcnew System::Windows::Forms::Button());
			this->label155 = (gcnew System::Windows::Forms::Label());
			this->textBox83 = (gcnew System::Windows::Forms::TextBox());
			this->button66 = (gcnew System::Windows::Forms::Button());
			this->label156 = (gcnew System::Windows::Forms::Label());
			this->textBox84 = (gcnew System::Windows::Forms::TextBox());
			this->label157 = (gcnew System::Windows::Forms::Label());
			this->button67 = (gcnew System::Windows::Forms::Button());
			this->label158 = (gcnew System::Windows::Forms::Label());
			this->button68 = (gcnew System::Windows::Forms::Button());
			this->button69 = (gcnew System::Windows::Forms::Button());
			this->textBox85 = (gcnew System::Windows::Forms::TextBox());
			this->textBox86 = (gcnew System::Windows::Forms::TextBox());
			this->button70 = (gcnew System::Windows::Forms::Button());
			this->textBox87 = (gcnew System::Windows::Forms::TextBox());
			this->button71 = (gcnew System::Windows::Forms::Button());
			this->textBox88 = (gcnew System::Windows::Forms::TextBox());
			this->label159 = (gcnew System::Windows::Forms::Label());
			this->label160 = (gcnew System::Windows::Forms::Label());
			this->label161 = (gcnew System::Windows::Forms::Label());
			this->button72 = (gcnew System::Windows::Forms::Button());
			this->textBox89 = (gcnew System::Windows::Forms::TextBox());
			this->button73 = (gcnew System::Windows::Forms::Button());
			this->textBox90 = (gcnew System::Windows::Forms::TextBox());
			this->button74 = (gcnew System::Windows::Forms::Button());
			this->textBox91 = (gcnew System::Windows::Forms::TextBox());
			this->label162 = (gcnew System::Windows::Forms::Label());
			this->label163 = (gcnew System::Windows::Forms::Label());
			this->label164 = (gcnew System::Windows::Forms::Label());
			this->groupBox19 = (gcnew System::Windows::Forms::GroupBox());
			this->button75 = (gcnew System::Windows::Forms::Button());
			this->label165 = (gcnew System::Windows::Forms::Label());
			this->textBox92 = (gcnew System::Windows::Forms::TextBox());
			this->tabPage56 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl14 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage57 = (gcnew System::Windows::Forms::TabPage());
			this->button76 = (gcnew System::Windows::Forms::Button());
			this->button77 = (gcnew System::Windows::Forms::Button());
			this->textBox93 = (gcnew System::Windows::Forms::TextBox());
			this->label166 = (gcnew System::Windows::Forms::Label());
			this->button78 = (gcnew System::Windows::Forms::Button());
			this->textBox94 = (gcnew System::Windows::Forms::TextBox());
			this->label167 = (gcnew System::Windows::Forms::Label());
			this->textBox95 = (gcnew System::Windows::Forms::TextBox());
			this->label168 = (gcnew System::Windows::Forms::Label());
			this->button79 = (gcnew System::Windows::Forms::Button());
			this->tabPage58 = (gcnew System::Windows::Forms::TabPage());
			this->button80 = (gcnew System::Windows::Forms::Button());
			this->groupBox20 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox20 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox21 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox22 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox23 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox24 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox25 = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl15 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage59 = (gcnew System::Windows::Forms::TabPage());
			this->textBox96 = (gcnew System::Windows::Forms::TextBox());
			this->label169 = (gcnew System::Windows::Forms::Label());
			this->tabPage60 = (gcnew System::Windows::Forms::TabPage());
			this->textBox97 = (gcnew System::Windows::Forms::TextBox());
			this->label170 = (gcnew System::Windows::Forms::Label());
			this->textBox98 = (gcnew System::Windows::Forms::TextBox());
			this->label171 = (gcnew System::Windows::Forms::Label());
			this->textBox99 = (gcnew System::Windows::Forms::TextBox());
			this->label172 = (gcnew System::Windows::Forms::Label());
			this->tabPage61 = (gcnew System::Windows::Forms::TabPage());
			this->textBox100 = (gcnew System::Windows::Forms::TextBox());
			this->label173 = (gcnew System::Windows::Forms::Label());
			this->textBox101 = (gcnew System::Windows::Forms::TextBox());
			this->label174 = (gcnew System::Windows::Forms::Label());
			this->tabPage62 = (gcnew System::Windows::Forms::TabPage());
			this->textBox102 = (gcnew System::Windows::Forms::TextBox());
			this->label175 = (gcnew System::Windows::Forms::Label());
			this->textBox103 = (gcnew System::Windows::Forms::TextBox());
			this->label176 = (gcnew System::Windows::Forms::Label());
			this->tabPage63 = (gcnew System::Windows::Forms::TabPage());
			this->textBox104 = (gcnew System::Windows::Forms::TextBox());
			this->label177 = (gcnew System::Windows::Forms::Label());
			this->textBox105 = (gcnew System::Windows::Forms::TextBox());
			this->label178 = (gcnew System::Windows::Forms::Label());
			this->textBox106 = (gcnew System::Windows::Forms::TextBox());
			this->label179 = (gcnew System::Windows::Forms::Label());
			this->groupBox21 = (gcnew System::Windows::Forms::GroupBox());
			this->button81 = (gcnew System::Windows::Forms::Button());
			this->label180 = (gcnew System::Windows::Forms::Label());
			this->textBox107 = (gcnew System::Windows::Forms::TextBox());
			this->textBox108 = (gcnew System::Windows::Forms::TextBox());
			this->label181 = (gcnew System::Windows::Forms::Label());
			this->button82 = (gcnew System::Windows::Forms::Button());
			this->tabPage64 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl16 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage65 = (gcnew System::Windows::Forms::TabPage());
			this->button83 = (gcnew System::Windows::Forms::Button());
			this->textBox109 = (gcnew System::Windows::Forms::TextBox());
			this->label182 = (gcnew System::Windows::Forms::Label());
			this->button84 = (gcnew System::Windows::Forms::Button());
			this->textBox110 = (gcnew System::Windows::Forms::TextBox());
			this->label183 = (gcnew System::Windows::Forms::Label());
			this->button85 = (gcnew System::Windows::Forms::Button());
			this->button86 = (gcnew System::Windows::Forms::Button());
			this->textBox111 = (gcnew System::Windows::Forms::TextBox());
			this->label184 = (gcnew System::Windows::Forms::Label());
			this->button87 = (gcnew System::Windows::Forms::Button());
			this->textBox112 = (gcnew System::Windows::Forms::TextBox());
			this->label185 = (gcnew System::Windows::Forms::Label());
			this->button88 = (gcnew System::Windows::Forms::Button());
			this->textBox113 = (gcnew System::Windows::Forms::TextBox());
			this->label186 = (gcnew System::Windows::Forms::Label());
			this->button89 = (gcnew System::Windows::Forms::Button());
			this->textBox114 = (gcnew System::Windows::Forms::TextBox());
			this->label187 = (gcnew System::Windows::Forms::Label());
			this->button90 = (gcnew System::Windows::Forms::Button());
			this->textBox115 = (gcnew System::Windows::Forms::TextBox());
			this->label188 = (gcnew System::Windows::Forms::Label());
			this->button91 = (gcnew System::Windows::Forms::Button());
			this->textBox116 = (gcnew System::Windows::Forms::TextBox());
			this->label189 = (gcnew System::Windows::Forms::Label());
			this->button92 = (gcnew System::Windows::Forms::Button());
			this->textBox117 = (gcnew System::Windows::Forms::TextBox());
			this->label190 = (gcnew System::Windows::Forms::Label());
			this->tabPage66 = (gcnew System::Windows::Forms::TabPage());
			this->button93 = (gcnew System::Windows::Forms::Button());
			this->groupBox22 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox26 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox27 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox28 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox29 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox30 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox31 = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl17 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage67 = (gcnew System::Windows::Forms::TabPage());
			this->textBox118 = (gcnew System::Windows::Forms::TextBox());
			this->label191 = (gcnew System::Windows::Forms::Label());
			this->tabPage68 = (gcnew System::Windows::Forms::TabPage());
			this->textBox119 = (gcnew System::Windows::Forms::TextBox());
			this->label192 = (gcnew System::Windows::Forms::Label());
			this->textBox120 = (gcnew System::Windows::Forms::TextBox());
			this->label193 = (gcnew System::Windows::Forms::Label());
			this->textBox121 = (gcnew System::Windows::Forms::TextBox());
			this->label194 = (gcnew System::Windows::Forms::Label());
			this->tabPage69 = (gcnew System::Windows::Forms::TabPage());
			this->textBox122 = (gcnew System::Windows::Forms::TextBox());
			this->label195 = (gcnew System::Windows::Forms::Label());
			this->textBox123 = (gcnew System::Windows::Forms::TextBox());
			this->label196 = (gcnew System::Windows::Forms::Label());
			this->tabPage70 = (gcnew System::Windows::Forms::TabPage());
			this->textBox124 = (gcnew System::Windows::Forms::TextBox());
			this->label197 = (gcnew System::Windows::Forms::Label());
			this->textBox125 = (gcnew System::Windows::Forms::TextBox());
			this->label198 = (gcnew System::Windows::Forms::Label());
			this->tabPage71 = (gcnew System::Windows::Forms::TabPage());
			this->textBox126 = (gcnew System::Windows::Forms::TextBox());
			this->label199 = (gcnew System::Windows::Forms::Label());
			this->textBox127 = (gcnew System::Windows::Forms::TextBox());
			this->label200 = (gcnew System::Windows::Forms::Label());
			this->textBox128 = (gcnew System::Windows::Forms::TextBox());
			this->label201 = (gcnew System::Windows::Forms::Label());
			this->groupBox23 = (gcnew System::Windows::Forms::GroupBox());
			this->button94 = (gcnew System::Windows::Forms::Button());
			this->label202 = (gcnew System::Windows::Forms::Label());
			this->textBox129 = (gcnew System::Windows::Forms::TextBox());
			this->textBox130 = (gcnew System::Windows::Forms::TextBox());
			this->label203 = (gcnew System::Windows::Forms::Label());
			this->button95 = (gcnew System::Windows::Forms::Button());
			this->tabPage72 = (gcnew System::Windows::Forms::TabPage());
			this->textBox131 = (gcnew System::Windows::Forms::TextBox());
			this->label204 = (gcnew System::Windows::Forms::Label());
			this->textBox132 = (gcnew System::Windows::Forms::TextBox());
			this->label205 = (gcnew System::Windows::Forms::Label());
			this->button96 = (gcnew System::Windows::Forms::Button());
			this->tabControl18 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage73 = (gcnew System::Windows::Forms::TabPage());
			this->button97 = (gcnew System::Windows::Forms::Button());
			this->textBox133 = (gcnew System::Windows::Forms::TextBox());
			this->label206 = (gcnew System::Windows::Forms::Label());
			this->button98 = (gcnew System::Windows::Forms::Button());
			this->textBox134 = (gcnew System::Windows::Forms::TextBox());
			this->label207 = (gcnew System::Windows::Forms::Label());
			this->button99 = (gcnew System::Windows::Forms::Button());
			this->textBox135 = (gcnew System::Windows::Forms::TextBox());
			this->label208 = (gcnew System::Windows::Forms::Label());
			this->button100 = (gcnew System::Windows::Forms::Button());
			this->textBox136 = (gcnew System::Windows::Forms::TextBox());
			this->label209 = (gcnew System::Windows::Forms::Label());
			this->button101 = (gcnew System::Windows::Forms::Button());
			this->button102 = (gcnew System::Windows::Forms::Button());
			this->textBox137 = (gcnew System::Windows::Forms::TextBox());
			this->label210 = (gcnew System::Windows::Forms::Label());
			this->button103 = (gcnew System::Windows::Forms::Button());
			this->textBox138 = (gcnew System::Windows::Forms::TextBox());
			this->label211 = (gcnew System::Windows::Forms::Label());
			this->button104 = (gcnew System::Windows::Forms::Button());
			this->textBox139 = (gcnew System::Windows::Forms::TextBox());
			this->label212 = (gcnew System::Windows::Forms::Label());
			this->button105 = (gcnew System::Windows::Forms::Button());
			this->textBox140 = (gcnew System::Windows::Forms::TextBox());
			this->label213 = (gcnew System::Windows::Forms::Label());
			this->tabPage74 = (gcnew System::Windows::Forms::TabPage());
			this->button106 = (gcnew System::Windows::Forms::Button());
			this->groupBox24 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox32 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox33 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox34 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox35 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox36 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox37 = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl19 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage75 = (gcnew System::Windows::Forms::TabPage());
			this->textBox141 = (gcnew System::Windows::Forms::TextBox());
			this->label214 = (gcnew System::Windows::Forms::Label());
			this->tabPage76 = (gcnew System::Windows::Forms::TabPage());
			this->textBox142 = (gcnew System::Windows::Forms::TextBox());
			this->label215 = (gcnew System::Windows::Forms::Label());
			this->textBox143 = (gcnew System::Windows::Forms::TextBox());
			this->label216 = (gcnew System::Windows::Forms::Label());
			this->textBox144 = (gcnew System::Windows::Forms::TextBox());
			this->label217 = (gcnew System::Windows::Forms::Label());
			this->tabPage77 = (gcnew System::Windows::Forms::TabPage());
			this->textBox145 = (gcnew System::Windows::Forms::TextBox());
			this->label218 = (gcnew System::Windows::Forms::Label());
			this->textBox146 = (gcnew System::Windows::Forms::TextBox());
			this->label219 = (gcnew System::Windows::Forms::Label());
			this->tabPage78 = (gcnew System::Windows::Forms::TabPage());
			this->textBox147 = (gcnew System::Windows::Forms::TextBox());
			this->label220 = (gcnew System::Windows::Forms::Label());
			this->textBox148 = (gcnew System::Windows::Forms::TextBox());
			this->label221 = (gcnew System::Windows::Forms::Label());
			this->tabPage79 = (gcnew System::Windows::Forms::TabPage());
			this->textBox149 = (gcnew System::Windows::Forms::TextBox());
			this->label222 = (gcnew System::Windows::Forms::Label());
			this->textBox150 = (gcnew System::Windows::Forms::TextBox());
			this->label223 = (gcnew System::Windows::Forms::Label());
			this->tabPage80 = (gcnew System::Windows::Forms::TabPage());
			this->button107 = (gcnew System::Windows::Forms::Button());
			this->button108 = (gcnew System::Windows::Forms::Button());
			this->label224 = (gcnew System::Windows::Forms::Label());
			this->textBox151 = (gcnew System::Windows::Forms::TextBox());
			this->textBox152 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox25 = (gcnew System::Windows::Forms::GroupBox());
			this->button109 = (gcnew System::Windows::Forms::Button());
			this->label225 = (gcnew System::Windows::Forms::Label());
			this->textBox153 = (gcnew System::Windows::Forms::TextBox());
			this->tabPage81 = (gcnew System::Windows::Forms::TabPage());
			this->button110 = (gcnew System::Windows::Forms::Button());
			this->label226 = (gcnew System::Windows::Forms::Label());
			this->textBox154 = (gcnew System::Windows::Forms::TextBox());
			this->button111 = (gcnew System::Windows::Forms::Button());
			this->textBox155 = (gcnew System::Windows::Forms::TextBox());
			this->checkBox38 = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage82 = (gcnew System::Windows::Forms::TabPage());
			this->ZmienNastawy = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl7 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage83 = (gcnew System::Windows::Forms::TabPage());
			this->zmienWyMinAPID = (gcnew System::Windows::Forms::Button());
			this->wyMinAPID = (gcnew System::Windows::Forms::TextBox());
			this->label227 = (gcnew System::Windows::Forms::Label());
			this->zmienWyMaxAPID = (gcnew System::Windows::Forms::Button());
			this->wyMaxAPID = (gcnew System::Windows::Forms::TextBox());
			this->label228 = (gcnew System::Windows::Forms::Label());
			this->zmienWszystkoAPID = (gcnew System::Windows::Forms::Button());
			this->zmienOnAPID = (gcnew System::Windows::Forms::Button());
			this->onAPID = (gcnew System::Windows::Forms::TextBox());
			this->label232 = (gcnew System::Windows::Forms::Label());
			this->zmienTdAPID = (gcnew System::Windows::Forms::Button());
			this->tdAPID = (gcnew System::Windows::Forms::TextBox());
			this->label233 = (gcnew System::Windows::Forms::Label());
			this->zmienTiAPID = (gcnew System::Windows::Forms::Button());
			this->tiAPID = (gcnew System::Windows::Forms::TextBox());
			this->label234 = (gcnew System::Windows::Forms::Label());
			this->zmienKAPID = (gcnew System::Windows::Forms::Button());
			this->kAPID = (gcnew System::Windows::Forms::TextBox());
			this->label235 = (gcnew System::Windows::Forms::Label());
			this->tabPage84 = (gcnew System::Windows::Forms::TabPage());
			this->zmienWymuszenieAPID = (gcnew System::Windows::Forms::Button());
			this->groupBox26 = (gcnew System::Windows::Forms::GroupBox());
			this->skokAPID = (gcnew System::Windows::Forms::CheckBox());
			this->szumAPID = (gcnew System::Windows::Forms::CheckBox());
			this->kronekerAPID = (gcnew System::Windows::Forms::CheckBox());
			this->trojkatAPID = (gcnew System::Windows::Forms::CheckBox());
			this->prostokatAPID = (gcnew System::Windows::Forms::CheckBox());
			this->sinusAPID = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage999 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage85 = (gcnew System::Windows::Forms::TabPage());
			this->nowySkokAPID = (gcnew System::Windows::Forms::TextBox());
			this->label236 = (gcnew System::Windows::Forms::Label());
			this->tabPage86 = (gcnew System::Windows::Forms::TabPage());
			this->noweWypelnieniePAPID = (gcnew System::Windows::Forms::TextBox());
			this->label237 = (gcnew System::Windows::Forms::Label());
			this->nowyOkresPAPID = (gcnew System::Windows::Forms::TextBox());
			this->label238 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaPAPID = (gcnew System::Windows::Forms::TextBox());
			this->label239 = (gcnew System::Windows::Forms::Label());
			this->tabPage87 = (gcnew System::Windows::Forms::TabPage());
			this->nowyOkresSAPID = (gcnew System::Windows::Forms::TextBox());
			this->label240 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaSAPID = (gcnew System::Windows::Forms::TextBox());
			this->label241 = (gcnew System::Windows::Forms::Label());
			this->tabPage88 = (gcnew System::Windows::Forms::TabPage());
			this->nowyOkresTAPID = (gcnew System::Windows::Forms::TextBox());
			this->label242 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaTAPID = (gcnew System::Windows::Forms::TextBox());
			this->label243 = (gcnew System::Windows::Forms::Label());
			this->tabPage89 = (gcnew System::Windows::Forms::TabPage());
			this->nowaLiczbaProbekAPID = (gcnew System::Windows::Forms::TextBox());
			this->label244 = (gcnew System::Windows::Forms::Label());
			this->nowaWariancjaAPID = (gcnew System::Windows::Forms::TextBox());
			this->label245 = (gcnew System::Windows::Forms::Label());
			this->tabPage19 = (gcnew System::Windows::Forms::TabPage());
			this->zmienAlfa = (gcnew System::Windows::Forms::Button());
			this->label54 = (gcnew System::Windows::Forms::Label());
			this->nowaAlfa = (gcnew System::Windows::Forms::TextBox());
			this->identyfikacja_textBox = (gcnew System::Windows::Forms::TextBox());
			this->czyscIdent = (gcnew System::Windows::Forms::Button());
			this->identyfikuj = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage18 = (gcnew System::Windows::Forms::TabPage());
			this->nazwaPlikuGPC = (gcnew System::Windows::Forms::TextBox());
			this->nazwaReg_GPC = (gcnew System::Windows::Forms::TextBox());
			this->label75 = (gcnew System::Windows::Forms::Label());
			this->label76 = (gcnew System::Windows::Forms::Label());
			this->zapiszGPC = (gcnew System::Windows::Forms::Button());
			this->tabControl5 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage20 = (gcnew System::Windows::Forms::TabPage());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->dBGPC = (gcnew System::Windows::Forms::TextBox());
			this->label59 = (gcnew System::Windows::Forms::Label());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->dAGPC = (gcnew System::Windows::Forms::TextBox());
			this->label60 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->wyMinGPC = (gcnew System::Windows::Forms::TextBox());
			this->label56 = (gcnew System::Windows::Forms::Label());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->wyMaxGPC = (gcnew System::Windows::Forms::TextBox());
			this->label57 = (gcnew System::Windows::Forms::Label());
			this->zmienWszystkoGPC = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->alfaGPC = (gcnew System::Windows::Forms::TextBox());
			this->label61 = (gcnew System::Windows::Forms::Label());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->roGPC = (gcnew System::Windows::Forms::TextBox());
			this->label62 = (gcnew System::Windows::Forms::Label());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->LGPC = (gcnew System::Windows::Forms::TextBox());
			this->label63 = (gcnew System::Windows::Forms::Label());
			this->button11 = (gcnew System::Windows::Forms::Button());
			this->HGPC = (gcnew System::Windows::Forms::TextBox());
			this->label64 = (gcnew System::Windows::Forms::Label());
			this->tabPage21 = (gcnew System::Windows::Forms::TabPage());
			this->zmienWymuszenieGPC = (gcnew System::Windows::Forms::Button());
			this->groupBox8 = (gcnew System::Windows::Forms::GroupBox());
			this->skokGPC = (gcnew System::Windows::Forms::CheckBox());
			this->szumGPC = (gcnew System::Windows::Forms::CheckBox());
			this->kronekerGPC = (gcnew System::Windows::Forms::CheckBox());
			this->trojkatGPC = (gcnew System::Windows::Forms::CheckBox());
			this->prostokatGPC = (gcnew System::Windows::Forms::CheckBox());
			this->sinusGPC = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl6 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage22 = (gcnew System::Windows::Forms::TabPage());
			this->nowySkokGPC = (gcnew System::Windows::Forms::TextBox());
			this->label65 = (gcnew System::Windows::Forms::Label());
			this->tabPage23 = (gcnew System::Windows::Forms::TabPage());
			this->noweWypelnieniePGPC = (gcnew System::Windows::Forms::TextBox());
			this->label66 = (gcnew System::Windows::Forms::Label());
			this->nowyOkresPGPC = (gcnew System::Windows::Forms::TextBox());
			this->label67 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaPGPC = (gcnew System::Windows::Forms::TextBox());
			this->label68 = (gcnew System::Windows::Forms::Label());
			this->tabPage24 = (gcnew System::Windows::Forms::TabPage());
			this->nowyOkresSGPC = (gcnew System::Windows::Forms::TextBox());
			this->label69 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaSGPC = (gcnew System::Windows::Forms::TextBox());
			this->label70 = (gcnew System::Windows::Forms::Label());
			this->tabPage25 = (gcnew System::Windows::Forms::TabPage());
			this->nowyOkresTGPC = (gcnew System::Windows::Forms::TextBox());
			this->label71 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaTGPC = (gcnew System::Windows::Forms::TextBox());
			this->label72 = (gcnew System::Windows::Forms::Label());
			this->tabPage26 = (gcnew System::Windows::Forms::TabPage());
			this->nowaLiczbaProbekGPC = (gcnew System::Windows::Forms::TextBox());
			this->label73 = (gcnew System::Windows::Forms::Label());
			this->nowaWariancjaGPC = (gcnew System::Windows::Forms::TextBox());
			this->label74 = (gcnew System::Windows::Forms::Label());
			this->tabPage27 = (gcnew System::Windows::Forms::TabPage());
			this->czyscIdentGPC = (gcnew System::Windows::Forms::Button());
			this->zmiennAlfaGPC = (gcnew System::Windows::Forms::Button());
			this->label58 = (gcnew System::Windows::Forms::Label());
			this->identyfikacjaGPC_textBox = (gcnew System::Windows::Forms::TextBox());
			this->nowaAlfaGPC = (gcnew System::Windows::Forms::TextBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->wczytajRegGPC = (gcnew System::Windows::Forms::Button());
			this->label55 = (gcnew System::Windows::Forms::Label());
			this->nazwaRegGPC = (gcnew System::Windows::Forms::TextBox());
			this->tabPage8 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl3 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage14 = (gcnew System::Windows::Forms::TabPage());
			this->zmienWyMinPID = (gcnew System::Windows::Forms::Button());
			this->wyMinPID = (gcnew System::Windows::Forms::TextBox());
			this->label50 = (gcnew System::Windows::Forms::Label());
			this->zmienWyMaxPID = (gcnew System::Windows::Forms::Button());
			this->wyMaxPID = (gcnew System::Windows::Forms::TextBox());
			this->label51 = (gcnew System::Windows::Forms::Label());
			this->zmienWszystkoPID = (gcnew System::Windows::Forms::Button());
			this->zmienBPID = (gcnew System::Windows::Forms::Button());
			this->bPID = (gcnew System::Windows::Forms::TextBox());
			this->label49 = (gcnew System::Windows::Forms::Label());
			this->zmienNPID = (gcnew System::Windows::Forms::Button());
			this->nPID = (gcnew System::Windows::Forms::TextBox());
			this->label48 = (gcnew System::Windows::Forms::Label());
			this->zmienWpPID = (gcnew System::Windows::Forms::Button());
			this->wpPID = (gcnew System::Windows::Forms::TextBox());
			this->label47 = (gcnew System::Windows::Forms::Label());
			this->zmienTpPID = (gcnew System::Windows::Forms::Button());
			this->tpPID = (gcnew System::Windows::Forms::TextBox());
			this->label46 = (gcnew System::Windows::Forms::Label());
			this->zmienTdPID = (gcnew System::Windows::Forms::Button());
			this->tdPID = (gcnew System::Windows::Forms::TextBox());
			this->label45 = (gcnew System::Windows::Forms::Label());
			this->zmienTiPID = (gcnew System::Windows::Forms::Button());
			this->tiPID = (gcnew System::Windows::Forms::TextBox());
			this->label44 = (gcnew System::Windows::Forms::Label());
			this->zmienKPID = (gcnew System::Windows::Forms::Button());
			this->kPID = (gcnew System::Windows::Forms::TextBox());
			this->label43 = (gcnew System::Windows::Forms::Label());
			this->tabPage15 = (gcnew System::Windows::Forms::TabPage());
			this->zmienWymuszeniePID = (gcnew System::Windows::Forms::Button());
			this->groupBox9 = (gcnew System::Windows::Forms::GroupBox());
			this->skokPID = (gcnew System::Windows::Forms::CheckBox());
			this->szumPID = (gcnew System::Windows::Forms::CheckBox());
			this->kronekerPID = (gcnew System::Windows::Forms::CheckBox());
			this->trojkatPID = (gcnew System::Windows::Forms::CheckBox());
			this->prostokatPID = (gcnew System::Windows::Forms::CheckBox());
			this->sinusPID = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl2 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage9 = (gcnew System::Windows::Forms::TabPage());
			this->nowySkokPID = (gcnew System::Windows::Forms::TextBox());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->tabPage10 = (gcnew System::Windows::Forms::TabPage());
			this->noweWypelnieniePPID = (gcnew System::Windows::Forms::TextBox());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->nowyOkresPPID = (gcnew System::Windows::Forms::TextBox());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaPPID = (gcnew System::Windows::Forms::TextBox());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->tabPage11 = (gcnew System::Windows::Forms::TabPage());
			this->nowyOkresSPID = (gcnew System::Windows::Forms::TextBox());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaSPID = (gcnew System::Windows::Forms::TextBox());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->tabPage12 = (gcnew System::Windows::Forms::TabPage());
			this->nowyOkresTPID = (gcnew System::Windows::Forms::TextBox());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaTPID = (gcnew System::Windows::Forms::TextBox());
			this->label38 = (gcnew System::Windows::Forms::Label());
			this->tabPage13 = (gcnew System::Windows::Forms::TabPage());
			this->nowaLiczbaProbekPID = (gcnew System::Windows::Forms::TextBox());
			this->label39 = (gcnew System::Windows::Forms::Label());
			this->nowaWariancjaPID = (gcnew System::Windows::Forms::TextBox());
			this->label40 = (gcnew System::Windows::Forms::Label());
			this->nazwaPlikuPID = (gcnew System::Windows::Forms::TextBox());
			this->nazwaReg_PID = (gcnew System::Windows::Forms::TextBox());
			this->label41 = (gcnew System::Windows::Forms::Label());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->wczytajRegPID = (gcnew System::Windows::Forms::Button());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->nazwaRegPID = (gcnew System::Windows::Forms::TextBox());
			this->label42 = (gcnew System::Windows::Forms::Label());
			this->zapiszPID = (gcnew System::Windows::Forms::Button());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl4 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage16 = (gcnew System::Windows::Forms::TabPage());
			this->zmienWszystkoP = (gcnew System::Windows::Forms::Button());
			this->zmienWyMinP = (gcnew System::Windows::Forms::Button());
			this->wyMinP = (gcnew System::Windows::Forms::TextBox());
			this->label52 = (gcnew System::Windows::Forms::Label());
			this->zmienWyMaxP = (gcnew System::Windows::Forms::Button());
			this->wyMaxP = (gcnew System::Windows::Forms::TextBox());
			this->label53 = (gcnew System::Windows::Forms::Label());
			this->k = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->zmienK = (gcnew System::Windows::Forms::Button());
			this->tabPage17 = (gcnew System::Windows::Forms::TabPage());
			this->zmienWymuszenieP = (gcnew System::Windows::Forms::Button());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->skokP = (gcnew System::Windows::Forms::CheckBox());
			this->szumP = (gcnew System::Windows::Forms::CheckBox());
			this->kronekerP = (gcnew System::Windows::Forms::CheckBox());
			this->trojkatP = (gcnew System::Windows::Forms::CheckBox());
			this->prostokatP = (gcnew System::Windows::Forms::CheckBox());
			this->sinusP = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->nowySkokP = (gcnew System::Windows::Forms::TextBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->noweWypelnieniePP = (gcnew System::Windows::Forms::TextBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->nowyOkresPP = (gcnew System::Windows::Forms::TextBox());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaPP = (gcnew System::Windows::Forms::TextBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->nowyOkresSP = (gcnew System::Windows::Forms::TextBox());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaSP = (gcnew System::Windows::Forms::TextBox());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->tabPage6 = (gcnew System::Windows::Forms::TabPage());
			this->nowyOkresTP = (gcnew System::Windows::Forms::TextBox());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->nowaAmplitudaTP = (gcnew System::Windows::Forms::TextBox());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->tabPage7 = (gcnew System::Windows::Forms::TabPage());
			this->nowaLiczbaProbekP = (gcnew System::Windows::Forms::TextBox());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->nowaWariancjaP = (gcnew System::Windows::Forms::TextBox());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->nazwaPlikuP = (gcnew System::Windows::Forms::TextBox());
			this->nazwaReg_P = (gcnew System::Windows::Forms::TextBox());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->wczytajRegP = (gcnew System::Windows::Forms::Button());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->nazwaRegP = (gcnew System::Windows::Forms::TextBox());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->zapiszP = (gcnew System::Windows::Forms::Button());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->zmienDlZakl = (gcnew System::Windows::Forms::Button());
			this->dlZakl = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->zmienWszystko = (gcnew System::Windows::Forms::Button());
			this->zmienI2 = (gcnew System::Windows::Forms::Button());
			this->i2 = (gcnew System::Windows::Forms::TextBox());
			this->zmienI1 = (gcnew System::Windows::Forms::Button());
			this->wyMin = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->i1 = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->zmienD = (gcnew System::Windows::Forms::Button());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->d = (gcnew System::Windows::Forms::TextBox());
			this->zmienWyMin = (gcnew System::Windows::Forms::Button());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->wyMax = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->zmienWariancja = (gcnew System::Windows::Forms::Button());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->zmienWyMax = (gcnew System::Windows::Forms::Button());
			this->zmienC2 = (gcnew System::Windows::Forms::Button());
			this->wariancja = (gcnew System::Windows::Forms::TextBox());
			this->c2 = (gcnew System::Windows::Forms::TextBox());
			this->zmienB2 = (gcnew System::Windows::Forms::Button());
			this->b2 = (gcnew System::Windows::Forms::TextBox());
			this->zmienA2 = (gcnew System::Windows::Forms::Button());
			this->a2 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->zmienC1 = (gcnew System::Windows::Forms::Button());
			this->c1 = (gcnew System::Windows::Forms::TextBox());
			this->zmienB1 = (gcnew System::Windows::Forms::Button());
			this->b1 = (gcnew System::Windows::Forms::TextBox());
			this->zmienA1 = (gcnew System::Windows::Forms::Button());
			this->a1 = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->wczytaj = (gcnew System::Windows::Forms::Button());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->nazwaObiektu = (gcnew System::Windows::Forms::TextBox());
			this->tabControl = (gcnew System::Windows::Forms::TabControl());
			this->wykres2 = (gcnew PSS_LAB::Wykres());
			this->wykres1 = (gcnew PSS_LAB::Wykres());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->czasKrokuTrackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->czasKrokuNumericUpDown))->BeginInit();
			this->tabPage28->SuspendLayout();
			this->groupBox10->SuspendLayout();
			this->groupBox11->SuspendLayout();
			this->tabPage29->SuspendLayout();
			this->tabControl8->SuspendLayout();
			this->tabPage30->SuspendLayout();
			this->tabPage31->SuspendLayout();
			this->groupBox12->SuspendLayout();
			this->tabControl9->SuspendLayout();
			this->tabPage32->SuspendLayout();
			this->tabPage33->SuspendLayout();
			this->tabPage34->SuspendLayout();
			this->tabPage35->SuspendLayout();
			this->tabPage36->SuspendLayout();
			this->groupBox13->SuspendLayout();
			this->tabPage37->SuspendLayout();
			this->tabControl10->SuspendLayout();
			this->tabPage38->SuspendLayout();
			this->tabPage39->SuspendLayout();
			this->groupBox14->SuspendLayout();
			this->tabControl11->SuspendLayout();
			this->tabPage40->SuspendLayout();
			this->tabPage41->SuspendLayout();
			this->tabPage42->SuspendLayout();
			this->tabPage43->SuspendLayout();
			this->tabPage44->SuspendLayout();
			this->groupBox15->SuspendLayout();
			this->tabPage45->SuspendLayout();
			this->tabControl12->SuspendLayout();
			this->tabPage46->SuspendLayout();
			this->tabPage47->SuspendLayout();
			this->groupBox16->SuspendLayout();
			this->tabControl13->SuspendLayout();
			this->tabPage48->SuspendLayout();
			this->tabPage49->SuspendLayout();
			this->tabPage50->SuspendLayout();
			this->tabPage51->SuspendLayout();
			this->tabPage52->SuspendLayout();
			this->tabPage53->SuspendLayout();
			this->groupBox17->SuspendLayout();
			this->tabPage54->SuspendLayout();
			this->tabPage55->SuspendLayout();
			this->groupBox18->SuspendLayout();
			this->groupBox19->SuspendLayout();
			this->tabPage56->SuspendLayout();
			this->tabControl14->SuspendLayout();
			this->tabPage57->SuspendLayout();
			this->tabPage58->SuspendLayout();
			this->groupBox20->SuspendLayout();
			this->tabControl15->SuspendLayout();
			this->tabPage59->SuspendLayout();
			this->tabPage60->SuspendLayout();
			this->tabPage61->SuspendLayout();
			this->tabPage62->SuspendLayout();
			this->tabPage63->SuspendLayout();
			this->groupBox21->SuspendLayout();
			this->tabPage64->SuspendLayout();
			this->tabControl16->SuspendLayout();
			this->tabPage65->SuspendLayout();
			this->tabPage66->SuspendLayout();
			this->groupBox22->SuspendLayout();
			this->tabControl17->SuspendLayout();
			this->tabPage67->SuspendLayout();
			this->tabPage68->SuspendLayout();
			this->tabPage69->SuspendLayout();
			this->tabPage70->SuspendLayout();
			this->tabPage71->SuspendLayout();
			this->groupBox23->SuspendLayout();
			this->tabPage72->SuspendLayout();
			this->tabControl18->SuspendLayout();
			this->tabPage73->SuspendLayout();
			this->tabPage74->SuspendLayout();
			this->groupBox24->SuspendLayout();
			this->tabControl19->SuspendLayout();
			this->tabPage75->SuspendLayout();
			this->tabPage76->SuspendLayout();
			this->tabPage77->SuspendLayout();
			this->tabPage78->SuspendLayout();
			this->tabPage79->SuspendLayout();
			this->tabPage80->SuspendLayout();
			this->groupBox25->SuspendLayout();
			this->tabPage81->SuspendLayout();
			this->tabPage82->SuspendLayout();
			this->tabControl7->SuspendLayout();
			this->tabPage83->SuspendLayout();
			this->tabPage84->SuspendLayout();
			this->groupBox26->SuspendLayout();
			this->tabPage999->SuspendLayout();
			this->tabPage85->SuspendLayout();
			this->tabPage86->SuspendLayout();
			this->tabPage87->SuspendLayout();
			this->tabPage88->SuspendLayout();
			this->tabPage89->SuspendLayout();
			this->tabPage19->SuspendLayout();
			this->tabPage18->SuspendLayout();
			this->tabControl5->SuspendLayout();
			this->tabPage20->SuspendLayout();
			this->tabPage21->SuspendLayout();
			this->groupBox8->SuspendLayout();
			this->tabControl6->SuspendLayout();
			this->tabPage22->SuspendLayout();
			this->tabPage23->SuspendLayout();
			this->tabPage24->SuspendLayout();
			this->tabPage25->SuspendLayout();
			this->tabPage26->SuspendLayout();
			this->tabPage27->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->tabPage8->SuspendLayout();
			this->tabControl3->SuspendLayout();
			this->tabPage14->SuspendLayout();
			this->tabPage15->SuspendLayout();
			this->groupBox9->SuspendLayout();
			this->tabControl2->SuspendLayout();
			this->tabPage9->SuspendLayout();
			this->tabPage10->SuspendLayout();
			this->tabPage11->SuspendLayout();
			this->tabPage12->SuspendLayout();
			this->tabPage13->SuspendLayout();
			this->groupBox7->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->tabControl4->SuspendLayout();
			this->tabPage16->SuspendLayout();
			this->tabPage17->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage3->SuspendLayout();
			this->tabPage4->SuspendLayout();
			this->tabPage5->SuspendLayout();
			this->tabPage6->SuspendLayout();
			this->tabPage7->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->tabControl->SuspendLayout();
			this->SuspendLayout();
			// 
			// start
			// 
			this->start->Location = System::Drawing::Point(463, 12);
			this->start->Name = L"start";
			this->start->Size = System::Drawing::Size(75, 23);
			this->start->TabIndex = 1;
			this->start->Text = L"Start";
			this->start->UseVisualStyleBackColor = true;
			this->start->Click += gcnew System::EventHandler(this, &Form1::start_Click);
			// 
			// stop
			// 
			this->stop->Appearance = System::Windows::Forms::Appearance::Button;
			this->stop->Enabled = false;
			this->stop->Location = System::Drawing::Point(544, 12);
			this->stop->Name = L"stop";
			this->stop->Size = System::Drawing::Size(75, 24);
			this->stop->TabIndex = 3;
			this->stop->Text = L"Stop";
			this->stop->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->stop->UseVisualStyleBackColor = true;
			this->stop->CheckedChanged += gcnew System::EventHandler(this, &Form1::stop_CheckedChanged);
			// 
			// reset
			// 
			this->reset->Location = System::Drawing::Point(625, 13);
			this->reset->Name = L"reset";
			this->reset->Size = System::Drawing::Size(75, 23);
			this->reset->TabIndex = 4;
			this->reset->Text = L"Reset";
			this->reset->UseVisualStyleBackColor = true;
			this->reset->Click += gcnew System::EventHandler(this, &Form1::reset_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->czasKrokuTrackBar);
			this->groupBox1->Controls->Add(this->czasKrokuNumericUpDown);
			this->groupBox1->ForeColor = System::Drawing::Color::Black;
			this->groupBox1->Location = System::Drawing::Point(464, 42);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(441, 100);
			this->groupBox1->TabIndex = 5;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Czas kroku [ms]";
			// 
			// czasKrokuTrackBar
			// 
			this->czasKrokuTrackBar->LargeChange = 100;
			this->czasKrokuTrackBar->Location = System::Drawing::Point(7, 20);
			this->czasKrokuTrackBar->Maximum = 1000;
			this->czasKrokuTrackBar->Name = L"czasKrokuTrackBar";
			this->czasKrokuTrackBar->Size = System::Drawing::Size(419, 45);
			this->czasKrokuTrackBar->TabIndex = 1;
			this->czasKrokuTrackBar->TickFrequency = 100;
			this->czasKrokuTrackBar->TickStyle = System::Windows::Forms::TickStyle::Both;
			this->czasKrokuTrackBar->Scroll += gcnew System::EventHandler(this, &Form1::czasKrokuTrackBar_Scroll);
			// 
			// czasKrokuNumericUpDown
			// 
			this->czasKrokuNumericUpDown->Location = System::Drawing::Point(7, 74);
			this->czasKrokuNumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->czasKrokuNumericUpDown->Name = L"czasKrokuNumericUpDown";
			this->czasKrokuNumericUpDown->Size = System::Drawing::Size(120, 20);
			this->czasKrokuNumericUpDown->TabIndex = 0;
			this->czasKrokuNumericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::czasKrokuNumericUpDown_ValueChanged);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(136, 45);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Wczytaj";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(6, 22);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(93, 13);
			this->label29->TabIndex = 1;
			this->label29->Text = L"Nazwa regulatora:";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(105, 19);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(106, 20);
			this->textBox1->TabIndex = 0;
			// 
			// typRegulatora
			// 
			this->typRegulatora->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->typRegulatora->FormattingEnabled = true;
			this->typRegulatora->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"P", L"PID", L"GPC", L"APID"});
			this->typRegulatora->Location = System::Drawing::Point(760, 15);
			this->typRegulatora->Name = L"typRegulatora";
			this->typRegulatora->Size = System::Drawing::Size(145, 21);
			this->typRegulatora->TabIndex = 12;
			this->typRegulatora->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::typRegulatora_SelectedIndexChanged);
			// 
			// tabPage28
			// 
			this->tabPage28->Controls->Add(this->groupBox10);
			this->tabPage28->Controls->Add(this->groupBox11);
			this->tabPage28->Location = System::Drawing::Point(4, 22);
			this->tabPage28->Name = L"tabPage28";
			this->tabPage28->Padding = System::Windows::Forms::Padding(3);
			this->tabPage28->Size = System::Drawing::Size(345, 494);
			this->tabPage28->TabIndex = 0;
			this->tabPage28->Text = L"Obiekt";
			this->tabPage28->UseVisualStyleBackColor = true;
			// 
			// groupBox10
			// 
			this->groupBox10->Controls->Add(this->button6);
			this->groupBox10->Controls->Add(this->textBox2);
			this->groupBox10->Controls->Add(this->label77);
			this->groupBox10->Controls->Add(this->button7);
			this->groupBox10->Controls->Add(this->button12);
			this->groupBox10->Controls->Add(this->textBox3);
			this->groupBox10->Controls->Add(this->button13);
			this->groupBox10->Controls->Add(this->textBox4);
			this->groupBox10->Controls->Add(this->label78);
			this->groupBox10->Controls->Add(this->textBox5);
			this->groupBox10->Controls->Add(this->label79);
			this->groupBox10->Controls->Add(this->button14);
			this->groupBox10->Controls->Add(this->label80);
			this->groupBox10->Controls->Add(this->textBox6);
			this->groupBox10->Controls->Add(this->button15);
			this->groupBox10->Controls->Add(this->label81);
			this->groupBox10->Controls->Add(this->textBox7);
			this->groupBox10->Controls->Add(this->label82);
			this->groupBox10->Controls->Add(this->button16);
			this->groupBox10->Controls->Add(this->label83);
			this->groupBox10->Controls->Add(this->button17);
			this->groupBox10->Controls->Add(this->button18);
			this->groupBox10->Controls->Add(this->textBox8);
			this->groupBox10->Controls->Add(this->textBox9);
			this->groupBox10->Controls->Add(this->button19);
			this->groupBox10->Controls->Add(this->textBox10);
			this->groupBox10->Controls->Add(this->button20);
			this->groupBox10->Controls->Add(this->textBox11);
			this->groupBox10->Controls->Add(this->label84);
			this->groupBox10->Controls->Add(this->label85);
			this->groupBox10->Controls->Add(this->label86);
			this->groupBox10->Controls->Add(this->button21);
			this->groupBox10->Controls->Add(this->textBox12);
			this->groupBox10->Controls->Add(this->button22);
			this->groupBox10->Controls->Add(this->textBox13);
			this->groupBox10->Controls->Add(this->button23);
			this->groupBox10->Controls->Add(this->textBox14);
			this->groupBox10->Controls->Add(this->label87);
			this->groupBox10->Controls->Add(this->label88);
			this->groupBox10->Controls->Add(this->label89);
			this->groupBox10->ForeColor = System::Drawing::Color::Black;
			this->groupBox10->Location = System::Drawing::Point(6, 6);
			this->groupBox10->Name = L"groupBox10";
			this->groupBox10->Size = System::Drawing::Size(333, 397);
			this->groupBox10->TabIndex = 9;
			this->groupBox10->TabStop = false;
			this->groupBox10->Text = L"Zmiana parametrów";
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(281, 327);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(46, 23);
			this->button6->TabIndex = 39;
			this->button6->Text = L"Zmieñ";
			this->button6->UseVisualStyleBackColor = true;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(117, 329);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(158, 20);
			this->textBox2->TabIndex = 38;
			// 
			// label77
			// 
			this->label77->AutoSize = true;
			this->label77->Location = System::Drawing::Point(6, 332);
			this->label77->Name = L"label77";
			this->label77->Size = System::Drawing::Size(105, 13);
			this->label77->TabIndex = 37;
			this->label77->Text = L"d³ugoœæ zak³ócenia:";
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(204, 368);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(123, 23);
			this->button7->TabIndex = 36;
			this->button7->Text = L"Zmieñ wszystko";
			this->button7->UseVisualStyleBackColor = true;
			// 
			// button12
			// 
			this->button12->Location = System::Drawing::Point(281, 223);
			this->button12->Name = L"button12";
			this->button12->Size = System::Drawing::Size(46, 23);
			this->button12->TabIndex = 35;
			this->button12->Text = L"Zmieñ";
			this->button12->UseVisualStyleBackColor = true;
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(117, 225);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(158, 20);
			this->textBox3->TabIndex = 34;
			// 
			// button13
			// 
			this->button13->Location = System::Drawing::Point(281, 197);
			this->button13->Name = L"button13";
			this->button13->Size = System::Drawing::Size(46, 23);
			this->button13->TabIndex = 33;
			this->button13->Text = L"Zmieñ";
			this->button13->UseVisualStyleBackColor = true;
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(117, 251);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(158, 20);
			this->textBox4->TabIndex = 21;
			// 
			// label78
			// 
			this->label78->AutoSize = true;
			this->label78->Location = System::Drawing::Point(7, 254);
			this->label78->Name = L"label78";
			this->label78->Size = System::Drawing::Size(40, 13);
			this->label78->TabIndex = 18;
			this->label78->Text = L"wyMin:";
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(117, 199);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(158, 20);
			this->textBox5->TabIndex = 32;
			// 
			// label79
			// 
			this->label79->AutoSize = true;
			this->label79->Location = System::Drawing::Point(6, 280);
			this->label79->Name = L"label79";
			this->label79->Size = System::Drawing::Size(43, 13);
			this->label79->TabIndex = 19;
			this->label79->Text = L"wyMax:";
			// 
			// button14
			// 
			this->button14->Location = System::Drawing::Point(281, 171);
			this->button14->Name = L"button14";
			this->button14->Size = System::Drawing::Size(46, 23);
			this->button14->TabIndex = 31;
			this->button14->Text = L"Zmieñ";
			this->button14->UseVisualStyleBackColor = true;
			// 
			// label80
			// 
			this->label80->AutoSize = true;
			this->label80->Location = System::Drawing::Point(6, 306);
			this->label80->Name = L"label80";
			this->label80->Size = System::Drawing::Size(55, 13);
			this->label80->TabIndex = 20;
			this->label80->Text = L"wariancja:";
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(117, 173);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(158, 20);
			this->textBox6->TabIndex = 30;
			// 
			// button15
			// 
			this->button15->Location = System::Drawing::Point(281, 249);
			this->button15->Name = L"button15";
			this->button15->Size = System::Drawing::Size(46, 23);
			this->button15->TabIndex = 22;
			this->button15->Text = L"Zmieñ";
			this->button15->UseVisualStyleBackColor = true;
			// 
			// label81
			// 
			this->label81->AutoSize = true;
			this->label81->Location = System::Drawing::Point(6, 228);
			this->label81->Name = L"label81";
			this->label81->Size = System::Drawing::Size(19, 13);
			this->label81->TabIndex = 29;
			this->label81->Text = L"I2:";
			// 
			// textBox7
			// 
			this->textBox7->Location = System::Drawing::Point(117, 277);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(158, 20);
			this->textBox7->TabIndex = 23;
			// 
			// label82
			// 
			this->label82->AutoSize = true;
			this->label82->Location = System::Drawing::Point(6, 202);
			this->label82->Name = L"label82";
			this->label82->Size = System::Drawing::Size(19, 13);
			this->label82->TabIndex = 28;
			this->label82->Text = L"I1:";
			// 
			// button16
			// 
			this->button16->Location = System::Drawing::Point(281, 301);
			this->button16->Name = L"button16";
			this->button16->Size = System::Drawing::Size(46, 23);
			this->button16->TabIndex = 26;
			this->button16->Text = L"Zmieñ";
			this->button16->UseVisualStyleBackColor = true;
			// 
			// label83
			// 
			this->label83->AutoSize = true;
			this->label83->Location = System::Drawing::Point(7, 176);
			this->label83->Name = L"label83";
			this->label83->Size = System::Drawing::Size(16, 13);
			this->label83->TabIndex = 27;
			this->label83->Text = L"d:";
			// 
			// button17
			// 
			this->button17->Location = System::Drawing::Point(281, 275);
			this->button17->Name = L"button17";
			this->button17->Size = System::Drawing::Size(46, 23);
			this->button17->TabIndex = 24;
			this->button17->Text = L"Zmieñ";
			this->button17->UseVisualStyleBackColor = true;
			// 
			// button18
			// 
			this->button18->Location = System::Drawing::Point(281, 145);
			this->button18->Name = L"button18";
			this->button18->Size = System::Drawing::Size(46, 23);
			this->button18->TabIndex = 17;
			this->button18->Text = L"Zmieñ";
			this->button18->UseVisualStyleBackColor = true;
			// 
			// textBox8
			// 
			this->textBox8->Location = System::Drawing::Point(117, 303);
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(158, 20);
			this->textBox8->TabIndex = 25;
			// 
			// textBox9
			// 
			this->textBox9->Location = System::Drawing::Point(117, 147);
			this->textBox9->Name = L"textBox9";
			this->textBox9->Size = System::Drawing::Size(158, 20);
			this->textBox9->TabIndex = 16;
			// 
			// button19
			// 
			this->button19->Location = System::Drawing::Point(281, 119);
			this->button19->Name = L"button19";
			this->button19->Size = System::Drawing::Size(46, 23);
			this->button19->TabIndex = 15;
			this->button19->Text = L"Zmieñ";
			this->button19->UseVisualStyleBackColor = true;
			// 
			// textBox10
			// 
			this->textBox10->Location = System::Drawing::Point(117, 121);
			this->textBox10->Name = L"textBox10";
			this->textBox10->Size = System::Drawing::Size(158, 20);
			this->textBox10->TabIndex = 14;
			// 
			// button20
			// 
			this->button20->Location = System::Drawing::Point(281, 93);
			this->button20->Name = L"button20";
			this->button20->Size = System::Drawing::Size(46, 23);
			this->button20->TabIndex = 13;
			this->button20->Text = L"Zmieñ";
			this->button20->UseVisualStyleBackColor = true;
			// 
			// textBox11
			// 
			this->textBox11->Location = System::Drawing::Point(117, 95);
			this->textBox11->Name = L"textBox11";
			this->textBox11->Size = System::Drawing::Size(158, 20);
			this->textBox11->TabIndex = 12;
			// 
			// label84
			// 
			this->label84->AutoSize = true;
			this->label84->Location = System::Drawing::Point(6, 150);
			this->label84->Name = L"label84";
			this->label84->Size = System::Drawing::Size(23, 13);
			this->label84->TabIndex = 11;
			this->label84->Text = L"C2:";
			// 
			// label85
			// 
			this->label85->AutoSize = true;
			this->label85->Location = System::Drawing::Point(6, 124);
			this->label85->Name = L"label85";
			this->label85->Size = System::Drawing::Size(23, 13);
			this->label85->TabIndex = 10;
			this->label85->Text = L"B2:";
			// 
			// label86
			// 
			this->label86->AutoSize = true;
			this->label86->Location = System::Drawing::Point(7, 98);
			this->label86->Name = L"label86";
			this->label86->Size = System::Drawing::Size(23, 13);
			this->label86->TabIndex = 9;
			this->label86->Text = L"A2:";
			// 
			// button21
			// 
			this->button21->Location = System::Drawing::Point(281, 67);
			this->button21->Name = L"button21";
			this->button21->Size = System::Drawing::Size(46, 23);
			this->button21->TabIndex = 8;
			this->button21->Text = L"Zmieñ";
			this->button21->UseVisualStyleBackColor = true;
			// 
			// textBox12
			// 
			this->textBox12->Location = System::Drawing::Point(117, 69);
			this->textBox12->Name = L"textBox12";
			this->textBox12->Size = System::Drawing::Size(158, 20);
			this->textBox12->TabIndex = 7;
			// 
			// button22
			// 
			this->button22->Location = System::Drawing::Point(281, 41);
			this->button22->Name = L"button22";
			this->button22->Size = System::Drawing::Size(46, 23);
			this->button22->TabIndex = 6;
			this->button22->Text = L"Zmieñ";
			this->button22->UseVisualStyleBackColor = true;
			// 
			// textBox13
			// 
			this->textBox13->Location = System::Drawing::Point(117, 43);
			this->textBox13->Name = L"textBox13";
			this->textBox13->Size = System::Drawing::Size(158, 20);
			this->textBox13->TabIndex = 5;
			// 
			// button23
			// 
			this->button23->Location = System::Drawing::Point(281, 15);
			this->button23->Name = L"button23";
			this->button23->Size = System::Drawing::Size(46, 23);
			this->button23->TabIndex = 4;
			this->button23->Text = L"Zmieñ";
			this->button23->UseVisualStyleBackColor = true;
			// 
			// textBox14
			// 
			this->textBox14->Location = System::Drawing::Point(117, 13);
			this->textBox14->Name = L"textBox14";
			this->textBox14->Size = System::Drawing::Size(158, 20);
			this->textBox14->TabIndex = 3;
			// 
			// label87
			// 
			this->label87->AutoSize = true;
			this->label87->Location = System::Drawing::Point(6, 72);
			this->label87->Name = L"label87";
			this->label87->Size = System::Drawing::Size(23, 13);
			this->label87->TabIndex = 2;
			this->label87->Text = L"C1:";
			// 
			// label88
			// 
			this->label88->AutoSize = true;
			this->label88->Location = System::Drawing::Point(6, 46);
			this->label88->Name = L"label88";
			this->label88->Size = System::Drawing::Size(23, 13);
			this->label88->TabIndex = 1;
			this->label88->Text = L"B1:";
			// 
			// label89
			// 
			this->label89->AutoSize = true;
			this->label89->Location = System::Drawing::Point(7, 20);
			this->label89->Name = L"label89";
			this->label89->Size = System::Drawing::Size(23, 13);
			this->label89->TabIndex = 0;
			this->label89->Text = L"A1:";
			// 
			// groupBox11
			// 
			this->groupBox11->Controls->Add(this->button24);
			this->groupBox11->Controls->Add(this->label90);
			this->groupBox11->Controls->Add(this->textBox15);
			this->groupBox11->ForeColor = System::Drawing::Color::Black;
			this->groupBox11->Location = System::Drawing::Point(6, 409);
			this->groupBox11->Name = L"groupBox11";
			this->groupBox11->Size = System::Drawing::Size(333, 79);
			this->groupBox11->TabIndex = 10;
			this->groupBox11->TabStop = false;
			this->groupBox11->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// button24
			// 
			this->button24->Location = System::Drawing::Point(252, 45);
			this->button24->Name = L"button24";
			this->button24->Size = System::Drawing::Size(75, 23);
			this->button24->TabIndex = 2;
			this->button24->Text = L"Wczytaj";
			this->button24->UseVisualStyleBackColor = true;
			// 
			// label90
			// 
			this->label90->AutoSize = true;
			this->label90->Location = System::Drawing::Point(6, 22);
			this->label90->Name = L"label90";
			this->label90->Size = System::Drawing::Size(81, 13);
			this->label90->TabIndex = 1;
			this->label90->Text = L"Nazwa obiektu:";
			// 
			// textBox15
			// 
			this->textBox15->Location = System::Drawing::Point(93, 19);
			this->textBox15->Name = L"textBox15";
			this->textBox15->Size = System::Drawing::Size(234, 20);
			this->textBox15->TabIndex = 0;
			// 
			// tabPage29
			// 
			this->tabPage29->Controls->Add(this->tabControl8);
			this->tabPage29->Controls->Add(this->textBox29);
			this->tabPage29->Controls->Add(this->label104);
			this->tabPage29->Controls->Add(this->groupBox13);
			this->tabPage29->Controls->Add(this->textBox31);
			this->tabPage29->Controls->Add(this->label106);
			this->tabPage29->Controls->Add(this->button31);
			this->tabPage29->Location = System::Drawing::Point(4, 22);
			this->tabPage29->Name = L"tabPage29";
			this->tabPage29->Padding = System::Windows::Forms::Padding(3);
			this->tabPage29->Size = System::Drawing::Size(345, 494);
			this->tabPage29->TabIndex = 1;
			this->tabPage29->Text = L"Regulator P";
			this->tabPage29->UseVisualStyleBackColor = true;
			// 
			// tabControl8
			// 
			this->tabControl8->Controls->Add(this->tabPage30);
			this->tabControl8->Controls->Add(this->tabPage31);
			this->tabControl8->Location = System::Drawing::Point(6, 91);
			this->tabControl8->Name = L"tabControl8";
			this->tabControl8->SelectedIndex = 0;
			this->tabControl8->Size = System::Drawing::Size(333, 308);
			this->tabControl8->TabIndex = 12;
			// 
			// tabPage30
			// 
			this->tabPage30->Controls->Add(this->button25);
			this->tabPage30->Controls->Add(this->button26);
			this->tabPage30->Controls->Add(this->textBox16);
			this->tabPage30->Controls->Add(this->label91);
			this->tabPage30->Controls->Add(this->button27);
			this->tabPage30->Controls->Add(this->textBox17);
			this->tabPage30->Controls->Add(this->label92);
			this->tabPage30->Controls->Add(this->textBox18);
			this->tabPage30->Controls->Add(this->label93);
			this->tabPage30->Controls->Add(this->button28);
			this->tabPage30->Location = System::Drawing::Point(4, 22);
			this->tabPage30->Name = L"tabPage30";
			this->tabPage30->Padding = System::Windows::Forms::Padding(3);
			this->tabPage30->Size = System::Drawing::Size(325, 282);
			this->tabPage30->TabIndex = 0;
			this->tabPage30->Text = L"Zmiana parametrów";
			this->tabPage30->UseVisualStyleBackColor = true;
			// 
			// button25
			// 
			this->button25->Location = System::Drawing::Point(196, 253);
			this->button25->Name = L"button25";
			this->button25->Size = System::Drawing::Size(123, 23);
			this->button25->TabIndex = 50;
			this->button25->Text = L"Zmieñ wszystko";
			this->button25->UseVisualStyleBackColor = true;
			// 
			// button26
			// 
			this->button26->Location = System::Drawing::Point(273, 56);
			this->button26->Name = L"button26";
			this->button26->Size = System::Drawing::Size(46, 23);
			this->button26->TabIndex = 49;
			this->button26->Text = L"Zmieñ";
			this->button26->UseVisualStyleBackColor = true;
			// 
			// textBox16
			// 
			this->textBox16->Location = System::Drawing::Point(58, 58);
			this->textBox16->Name = L"textBox16";
			this->textBox16->Size = System::Drawing::Size(209, 20);
			this->textBox16->TabIndex = 48;
			// 
			// label91
			// 
			this->label91->AutoSize = true;
			this->label91->Location = System::Drawing::Point(3, 61);
			this->label91->Name = L"label91";
			this->label91->Size = System::Drawing::Size(49, 13);
			this->label91->TabIndex = 47;
			this->label91->Text = L"Wy_Min:";
			// 
			// button27
			// 
			this->button27->Location = System::Drawing::Point(273, 30);
			this->button27->Name = L"button27";
			this->button27->Size = System::Drawing::Size(46, 23);
			this->button27->TabIndex = 46;
			this->button27->Text = L"Zmieñ";
			this->button27->UseVisualStyleBackColor = true;
			// 
			// textBox17
			// 
			this->textBox17->Location = System::Drawing::Point(58, 32);
			this->textBox17->Name = L"textBox17";
			this->textBox17->Size = System::Drawing::Size(209, 20);
			this->textBox17->TabIndex = 45;
			// 
			// label92
			// 
			this->label92->AutoSize = true;
			this->label92->Location = System::Drawing::Point(3, 35);
			this->label92->Name = L"label92";
			this->label92->Size = System::Drawing::Size(52, 13);
			this->label92->TabIndex = 44;
			this->label92->Text = L"Wy_Max:";
			// 
			// textBox18
			// 
			this->textBox18->Location = System::Drawing::Point(58, 6);
			this->textBox18->Name = L"textBox18";
			this->textBox18->Size = System::Drawing::Size(209, 20);
			this->textBox18->TabIndex = 14;
			// 
			// label93
			// 
			this->label93->AutoSize = true;
			this->label93->Location = System::Drawing::Point(3, 9);
			this->label93->Name = L"label93";
			this->label93->Size = System::Drawing::Size(16, 13);
			this->label93->TabIndex = 13;
			this->label93->Text = L"k:";
			// 
			// button28
			// 
			this->button28->Location = System::Drawing::Point(273, 4);
			this->button28->Name = L"button28";
			this->button28->Size = System::Drawing::Size(46, 23);
			this->button28->TabIndex = 12;
			this->button28->Text = L"Zmieñ";
			this->button28->UseVisualStyleBackColor = true;
			// 
			// tabPage31
			// 
			this->tabPage31->Controls->Add(this->button29);
			this->tabPage31->Controls->Add(this->groupBox12);
			this->tabPage31->Controls->Add(this->tabControl9);
			this->tabPage31->Location = System::Drawing::Point(4, 22);
			this->tabPage31->Name = L"tabPage31";
			this->tabPage31->Padding = System::Windows::Forms::Padding(3);
			this->tabPage31->Size = System::Drawing::Size(325, 282);
			this->tabPage31->TabIndex = 1;
			this->tabPage31->Text = L"Zmiana wymuszenia";
			this->tabPage31->UseVisualStyleBackColor = true;
			// 
			// button29
			// 
			this->button29->Location = System::Drawing::Point(244, 253);
			this->button29->Name = L"button29";
			this->button29->Size = System::Drawing::Size(75, 23);
			this->button29->TabIndex = 9;
			this->button29->Text = L"Zmieñ";
			this->button29->UseVisualStyleBackColor = true;
			// 
			// groupBox12
			// 
			this->groupBox12->Controls->Add(this->checkBox1);
			this->groupBox12->Controls->Add(this->checkBox2);
			this->groupBox12->Controls->Add(this->checkBox3);
			this->groupBox12->Controls->Add(this->checkBox4);
			this->groupBox12->Controls->Add(this->checkBox5);
			this->groupBox12->Controls->Add(this->checkBox6);
			this->groupBox12->ForeColor = System::Drawing::Color::Black;
			this->groupBox12->Location = System::Drawing::Point(3, 8);
			this->groupBox12->Name = L"groupBox12";
			this->groupBox12->Size = System::Drawing::Size(316, 89);
			this->groupBox12->TabIndex = 7;
			this->groupBox12->TabStop = false;
			this->groupBox12->Text = L"Sk³adowe wymuszenia";
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(6, 19);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(51, 17);
			this->checkBox1->TabIndex = 1;
			this->checkBox1->Text = L"Skok";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Location = System::Drawing::Point(128, 65);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(52, 17);
			this->checkBox2->TabIndex = 6;
			this->checkBox2->Text = L"Szum";
			this->checkBox2->UseVisualStyleBackColor = true;
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Location = System::Drawing::Point(6, 42);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(108, 17);
			this->checkBox3->TabIndex = 2;
			this->checkBox3->Text = L"Impuls Kronekera";
			this->checkBox3->UseVisualStyleBackColor = true;
			// 
			// checkBox4
			// 
			this->checkBox4->AutoSize = true;
			this->checkBox4->Location = System::Drawing::Point(128, 42);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(59, 17);
			this->checkBox4->TabIndex = 5;
			this->checkBox4->Text = L"Trójk¹t";
			this->checkBox4->UseVisualStyleBackColor = true;
			// 
			// checkBox5
			// 
			this->checkBox5->AutoSize = true;
			this->checkBox5->Location = System::Drawing::Point(6, 65);
			this->checkBox5->Name = L"checkBox5";
			this->checkBox5->Size = System::Drawing::Size(71, 17);
			this->checkBox5->TabIndex = 3;
			this->checkBox5->Text = L"Prostok¹t";
			this->checkBox5->UseVisualStyleBackColor = true;
			// 
			// checkBox6
			// 
			this->checkBox6->AutoSize = true;
			this->checkBox6->Location = System::Drawing::Point(128, 19);
			this->checkBox6->Name = L"checkBox6";
			this->checkBox6->Size = System::Drawing::Size(52, 17);
			this->checkBox6->TabIndex = 4;
			this->checkBox6->Text = L"Sinus";
			this->checkBox6->UseVisualStyleBackColor = true;
			// 
			// tabControl9
			// 
			this->tabControl9->Controls->Add(this->tabPage32);
			this->tabControl9->Controls->Add(this->tabPage33);
			this->tabControl9->Controls->Add(this->tabPage34);
			this->tabControl9->Controls->Add(this->tabPage35);
			this->tabControl9->Controls->Add(this->tabPage36);
			this->tabControl9->Location = System::Drawing::Point(2, 103);
			this->tabControl9->Multiline = true;
			this->tabControl9->Name = L"tabControl9";
			this->tabControl9->SelectedIndex = 0;
			this->tabControl9->Size = System::Drawing::Size(317, 112);
			this->tabControl9->TabIndex = 8;
			// 
			// tabPage32
			// 
			this->tabPage32->Controls->Add(this->textBox19);
			this->tabPage32->Controls->Add(this->label94);
			this->tabPage32->Location = System::Drawing::Point(4, 22);
			this->tabPage32->Name = L"tabPage32";
			this->tabPage32->Padding = System::Windows::Forms::Padding(3);
			this->tabPage32->Size = System::Drawing::Size(309, 86);
			this->tabPage32->TabIndex = 0;
			this->tabPage32->Text = L"Skok";
			this->tabPage32->UseVisualStyleBackColor = true;
			// 
			// textBox19
			// 
			this->textBox19->Location = System::Drawing::Point(91, 6);
			this->textBox19->Name = L"textBox19";
			this->textBox19->Size = System::Drawing::Size(99, 20);
			this->textBox19->TabIndex = 1;
			// 
			// label94
			// 
			this->label94->AutoSize = true;
			this->label94->Location = System::Drawing::Point(6, 9);
			this->label94->Name = L"label94";
			this->label94->Size = System::Drawing::Size(79, 13);
			this->label94->TabIndex = 0;
			this->label94->Text = L"wartoœæ skoku:";
			// 
			// tabPage33
			// 
			this->tabPage33->Controls->Add(this->textBox20);
			this->tabPage33->Controls->Add(this->label95);
			this->tabPage33->Controls->Add(this->textBox21);
			this->tabPage33->Controls->Add(this->label96);
			this->tabPage33->Controls->Add(this->textBox22);
			this->tabPage33->Controls->Add(this->label97);
			this->tabPage33->Location = System::Drawing::Point(4, 22);
			this->tabPage33->Name = L"tabPage33";
			this->tabPage33->Padding = System::Windows::Forms::Padding(3);
			this->tabPage33->Size = System::Drawing::Size(309, 86);
			this->tabPage33->TabIndex = 1;
			this->tabPage33->Text = L"Prostok¹t";
			this->tabPage33->UseVisualStyleBackColor = true;
			// 
			// textBox20
			// 
			this->textBox20->Location = System::Drawing::Point(79, 58);
			this->textBox20->Name = L"textBox20";
			this->textBox20->Size = System::Drawing::Size(111, 20);
			this->textBox20->TabIndex = 7;
			// 
			// label95
			// 
			this->label95->AutoSize = true;
			this->label95->Location = System::Drawing::Point(6, 61);
			this->label95->Name = L"label95";
			this->label95->Size = System::Drawing::Size(67, 13);
			this->label95->TabIndex = 6;
			this->label95->Text = L"wype³nienie:";
			// 
			// textBox21
			// 
			this->textBox21->Location = System::Drawing::Point(79, 32);
			this->textBox21->Name = L"textBox21";
			this->textBox21->Size = System::Drawing::Size(111, 20);
			this->textBox21->TabIndex = 5;
			// 
			// label96
			// 
			this->label96->AutoSize = true;
			this->label96->Location = System::Drawing::Point(6, 35);
			this->label96->Name = L"label96";
			this->label96->Size = System::Drawing::Size(36, 13);
			this->label96->TabIndex = 4;
			this->label96->Text = L"okres:";
			// 
			// textBox22
			// 
			this->textBox22->Location = System::Drawing::Point(79, 6);
			this->textBox22->Name = L"textBox22";
			this->textBox22->Size = System::Drawing::Size(111, 20);
			this->textBox22->TabIndex = 3;
			// 
			// label97
			// 
			this->label97->AutoSize = true;
			this->label97->Location = System::Drawing::Point(6, 9);
			this->label97->Name = L"label97";
			this->label97->Size = System::Drawing::Size(55, 13);
			this->label97->TabIndex = 2;
			this->label97->Text = L"amplituda:";
			// 
			// tabPage34
			// 
			this->tabPage34->Controls->Add(this->textBox23);
			this->tabPage34->Controls->Add(this->label98);
			this->tabPage34->Controls->Add(this->textBox24);
			this->tabPage34->Controls->Add(this->label99);
			this->tabPage34->Location = System::Drawing::Point(4, 22);
			this->tabPage34->Name = L"tabPage34";
			this->tabPage34->Size = System::Drawing::Size(309, 86);
			this->tabPage34->TabIndex = 2;
			this->tabPage34->Text = L"Sinus";
			this->tabPage34->UseVisualStyleBackColor = true;
			// 
			// textBox23
			// 
			this->textBox23->Location = System::Drawing::Point(67, 32);
			this->textBox23->Name = L"textBox23";
			this->textBox23->Size = System::Drawing::Size(123, 20);
			this->textBox23->TabIndex = 9;
			// 
			// label98
			// 
			this->label98->AutoSize = true;
			this->label98->Location = System::Drawing::Point(6, 35);
			this->label98->Name = L"label98";
			this->label98->Size = System::Drawing::Size(36, 13);
			this->label98->TabIndex = 8;
			this->label98->Text = L"okres:";
			// 
			// textBox24
			// 
			this->textBox24->Location = System::Drawing::Point(67, 6);
			this->textBox24->Name = L"textBox24";
			this->textBox24->Size = System::Drawing::Size(123, 20);
			this->textBox24->TabIndex = 7;
			// 
			// label99
			// 
			this->label99->AutoSize = true;
			this->label99->Location = System::Drawing::Point(6, 9);
			this->label99->Name = L"label99";
			this->label99->Size = System::Drawing::Size(55, 13);
			this->label99->TabIndex = 6;
			this->label99->Text = L"amplituda:";
			// 
			// tabPage35
			// 
			this->tabPage35->Controls->Add(this->textBox25);
			this->tabPage35->Controls->Add(this->label100);
			this->tabPage35->Controls->Add(this->textBox26);
			this->tabPage35->Controls->Add(this->label101);
			this->tabPage35->Location = System::Drawing::Point(4, 22);
			this->tabPage35->Name = L"tabPage35";
			this->tabPage35->Size = System::Drawing::Size(309, 86);
			this->tabPage35->TabIndex = 3;
			this->tabPage35->Text = L"Trójk¹t";
			this->tabPage35->UseVisualStyleBackColor = true;
			// 
			// textBox25
			// 
			this->textBox25->Location = System::Drawing::Point(67, 32);
			this->textBox25->Name = L"textBox25";
			this->textBox25->Size = System::Drawing::Size(123, 20);
			this->textBox25->TabIndex = 13;
			// 
			// label100
			// 
			this->label100->AutoSize = true;
			this->label100->Location = System::Drawing::Point(6, 35);
			this->label100->Name = L"label100";
			this->label100->Size = System::Drawing::Size(36, 13);
			this->label100->TabIndex = 12;
			this->label100->Text = L"okres:";
			// 
			// textBox26
			// 
			this->textBox26->Location = System::Drawing::Point(67, 6);
			this->textBox26->Name = L"textBox26";
			this->textBox26->Size = System::Drawing::Size(123, 20);
			this->textBox26->TabIndex = 11;
			// 
			// label101
			// 
			this->label101->AutoSize = true;
			this->label101->Location = System::Drawing::Point(6, 9);
			this->label101->Name = L"label101";
			this->label101->Size = System::Drawing::Size(55, 13);
			this->label101->TabIndex = 10;
			this->label101->Text = L"amplituda:";
			// 
			// tabPage36
			// 
			this->tabPage36->Controls->Add(this->textBox27);
			this->tabPage36->Controls->Add(this->label102);
			this->tabPage36->Controls->Add(this->textBox28);
			this->tabPage36->Controls->Add(this->label103);
			this->tabPage36->Location = System::Drawing::Point(4, 22);
			this->tabPage36->Name = L"tabPage36";
			this->tabPage36->Size = System::Drawing::Size(309, 86);
			this->tabPage36->TabIndex = 4;
			this->tabPage36->Text = L"Szum";
			this->tabPage36->UseVisualStyleBackColor = true;
			// 
			// textBox27
			// 
			this->textBox27->Location = System::Drawing::Point(82, 32);
			this->textBox27->Name = L"textBox27";
			this->textBox27->Size = System::Drawing::Size(108, 20);
			this->textBox27->TabIndex = 13;
			// 
			// label102
			// 
			this->label102->AutoSize = true;
			this->label102->Location = System::Drawing::Point(6, 35);
			this->label102->Name = L"label102";
			this->label102->Size = System::Drawing::Size(73, 13);
			this->label102->TabIndex = 12;
			this->label102->Text = L"liczba próbek:";
			// 
			// textBox28
			// 
			this->textBox28->Location = System::Drawing::Point(82, 7);
			this->textBox28->Name = L"textBox28";
			this->textBox28->Size = System::Drawing::Size(108, 20);
			this->textBox28->TabIndex = 11;
			// 
			// label103
			// 
			this->label103->AutoSize = true;
			this->label103->Location = System::Drawing::Point(6, 10);
			this->label103->Name = L"label103";
			this->label103->Size = System::Drawing::Size(55, 13);
			this->label103->TabIndex = 10;
			this->label103->Text = L"wariancja:";
			// 
			// textBox29
			// 
			this->textBox29->Location = System::Drawing::Point(98, 431);
			this->textBox29->Name = L"textBox29";
			this->textBox29->Size = System::Drawing::Size(237, 20);
			this->textBox29->TabIndex = 14;
			// 
			// label104
			// 
			this->label104->AutoSize = true;
			this->label104->Location = System::Drawing::Point(5, 434);
			this->label104->Name = L"label104";
			this->label104->Size = System::Drawing::Size(27, 13);
			this->label104->TabIndex = 13;
			this->label104->Text = L"Plik:";
			// 
			// groupBox13
			// 
			this->groupBox13->Controls->Add(this->button30);
			this->groupBox13->Controls->Add(this->label105);
			this->groupBox13->Controls->Add(this->textBox30);
			this->groupBox13->ForeColor = System::Drawing::Color::Black;
			this->groupBox13->Location = System::Drawing::Point(6, 6);
			this->groupBox13->Name = L"groupBox13";
			this->groupBox13->Size = System::Drawing::Size(333, 79);
			this->groupBox13->TabIndex = 15;
			this->groupBox13->TabStop = false;
			this->groupBox13->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// button30
			// 
			this->button30->Location = System::Drawing::Point(252, 45);
			this->button30->Name = L"button30";
			this->button30->Size = System::Drawing::Size(75, 23);
			this->button30->TabIndex = 2;
			this->button30->Text = L"Wczytaj";
			this->button30->UseVisualStyleBackColor = true;
			// 
			// label105
			// 
			this->label105->AutoSize = true;
			this->label105->Location = System::Drawing::Point(6, 22);
			this->label105->Name = L"label105";
			this->label105->Size = System::Drawing::Size(93, 13);
			this->label105->TabIndex = 1;
			this->label105->Text = L"Nazwa regulatora:";
			// 
			// textBox30
			// 
			this->textBox30->Location = System::Drawing::Point(105, 19);
			this->textBox30->Name = L"textBox30";
			this->textBox30->Size = System::Drawing::Size(222, 20);
			this->textBox30->TabIndex = 0;
			// 
			// textBox31
			// 
			this->textBox31->Location = System::Drawing::Point(98, 405);
			this->textBox31->Name = L"textBox31";
			this->textBox31->Size = System::Drawing::Size(237, 20);
			this->textBox31->TabIndex = 12;
			// 
			// label106
			// 
			this->label106->AutoSize = true;
			this->label106->Location = System::Drawing::Point(4, 408);
			this->label106->Name = L"label106";
			this->label106->Size = System::Drawing::Size(93, 13);
			this->label106->TabIndex = 11;
			this->label106->Text = L"Nazwa regulatora:";
			// 
			// button31
			// 
			this->button31->Location = System::Drawing::Point(245, 457);
			this->button31->Name = L"button31";
			this->button31->Size = System::Drawing::Size(90, 23);
			this->button31->TabIndex = 10;
			this->button31->Text = L"Zapisz";
			this->button31->UseVisualStyleBackColor = true;
			// 
			// tabPage37
			// 
			this->tabPage37->Controls->Add(this->tabControl10);
			this->tabPage37->Controls->Add(this->textBox51);
			this->tabPage37->Controls->Add(this->label126);
			this->tabPage37->Controls->Add(this->groupBox15);
			this->tabPage37->Controls->Add(this->textBox53);
			this->tabPage37->Controls->Add(this->label128);
			this->tabPage37->Controls->Add(this->button44);
			this->tabPage37->Location = System::Drawing::Point(4, 22);
			this->tabPage37->Name = L"tabPage37";
			this->tabPage37->Size = System::Drawing::Size(345, 494);
			this->tabPage37->TabIndex = 2;
			this->tabPage37->Text = L"Regulator PID";
			this->tabPage37->UseVisualStyleBackColor = true;
			// 
			// tabControl10
			// 
			this->tabControl10->Controls->Add(this->tabPage38);
			this->tabControl10->Controls->Add(this->tabPage39);
			this->tabControl10->Location = System::Drawing::Point(6, 91);
			this->tabControl10->Name = L"tabControl10";
			this->tabControl10->SelectedIndex = 0;
			this->tabControl10->Size = System::Drawing::Size(333, 308);
			this->tabControl10->TabIndex = 20;
			// 
			// tabPage38
			// 
			this->tabPage38->Controls->Add(this->button32);
			this->tabPage38->Controls->Add(this->textBox32);
			this->tabPage38->Controls->Add(this->label107);
			this->tabPage38->Controls->Add(this->button33);
			this->tabPage38->Controls->Add(this->textBox33);
			this->tabPage38->Controls->Add(this->label108);
			this->tabPage38->Controls->Add(this->button34);
			this->tabPage38->Controls->Add(this->button35);
			this->tabPage38->Controls->Add(this->textBox34);
			this->tabPage38->Controls->Add(this->label109);
			this->tabPage38->Controls->Add(this->button36);
			this->tabPage38->Controls->Add(this->textBox35);
			this->tabPage38->Controls->Add(this->label110);
			this->tabPage38->Controls->Add(this->button37);
			this->tabPage38->Controls->Add(this->textBox36);
			this->tabPage38->Controls->Add(this->label111);
			this->tabPage38->Controls->Add(this->button38);
			this->tabPage38->Controls->Add(this->textBox37);
			this->tabPage38->Controls->Add(this->label112);
			this->tabPage38->Controls->Add(this->button39);
			this->tabPage38->Controls->Add(this->textBox38);
			this->tabPage38->Controls->Add(this->label113);
			this->tabPage38->Controls->Add(this->button40);
			this->tabPage38->Controls->Add(this->textBox39);
			this->tabPage38->Controls->Add(this->label114);
			this->tabPage38->Controls->Add(this->button41);
			this->tabPage38->Controls->Add(this->textBox40);
			this->tabPage38->Controls->Add(this->label115);
			this->tabPage38->Location = System::Drawing::Point(4, 22);
			this->tabPage38->Name = L"tabPage38";
			this->tabPage38->Padding = System::Windows::Forms::Padding(3);
			this->tabPage38->Size = System::Drawing::Size(325, 282);
			this->tabPage38->TabIndex = 0;
			this->tabPage38->Text = L"Zmiana parametrów";
			this->tabPage38->UseVisualStyleBackColor = true;
			// 
			// button32
			// 
			this->button32->Location = System::Drawing::Point(273, 212);
			this->button32->Name = L"button32";
			this->button32->Size = System::Drawing::Size(46, 23);
			this->button32->TabIndex = 43;
			this->button32->Text = L"Zmieñ";
			this->button32->UseVisualStyleBackColor = true;
			// 
			// textBox32
			// 
			this->textBox32->Location = System::Drawing::Point(101, 214);
			this->textBox32->Name = L"textBox32";
			this->textBox32->Size = System::Drawing::Size(166, 20);
			this->textBox32->TabIndex = 42;
			// 
			// label107
			// 
			this->label107->AutoSize = true;
			this->label107->Location = System::Drawing::Point(3, 217);
			this->label107->Name = L"label107";
			this->label107->Size = System::Drawing::Size(49, 13);
			this->label107->TabIndex = 41;
			this->label107->Text = L"Wy_Min:";
			// 
			// button33
			// 
			this->button33->Location = System::Drawing::Point(273, 186);
			this->button33->Name = L"button33";
			this->button33->Size = System::Drawing::Size(46, 23);
			this->button33->TabIndex = 40;
			this->button33->Text = L"Zmieñ";
			this->button33->UseVisualStyleBackColor = true;
			// 
			// textBox33
			// 
			this->textBox33->Location = System::Drawing::Point(101, 188);
			this->textBox33->Name = L"textBox33";
			this->textBox33->Size = System::Drawing::Size(166, 20);
			this->textBox33->TabIndex = 39;
			// 
			// label108
			// 
			this->label108->AutoSize = true;
			this->label108->Location = System::Drawing::Point(3, 191);
			this->label108->Name = L"label108";
			this->label108->Size = System::Drawing::Size(52, 13);
			this->label108->TabIndex = 38;
			this->label108->Text = L"Wy_Max:";
			// 
			// button34
			// 
			this->button34->Location = System::Drawing::Point(196, 253);
			this->button34->Name = L"button34";
			this->button34->Size = System::Drawing::Size(123, 23);
			this->button34->TabIndex = 37;
			this->button34->Text = L"Zmieñ wszystko";
			this->button34->UseVisualStyleBackColor = true;
			// 
			// button35
			// 
			this->button35->Location = System::Drawing::Point(273, 160);
			this->button35->Name = L"button35";
			this->button35->Size = System::Drawing::Size(46, 23);
			this->button35->TabIndex = 25;
			this->button35->Text = L"Zmieñ";
			this->button35->UseVisualStyleBackColor = true;
			// 
			// textBox34
			// 
			this->textBox34->Location = System::Drawing::Point(101, 162);
			this->textBox34->Name = L"textBox34";
			this->textBox34->Size = System::Drawing::Size(166, 20);
			this->textBox34->TabIndex = 24;
			// 
			// label109
			// 
			this->label109->AutoSize = true;
			this->label109->Location = System::Drawing::Point(3, 165);
			this->label109->Name = L"label109";
			this->label109->Size = System::Drawing::Size(16, 13);
			this->label109->TabIndex = 23;
			this->label109->Text = L"b:";
			// 
			// button36
			// 
			this->button36->Location = System::Drawing::Point(273, 134);
			this->button36->Name = L"button36";
			this->button36->Size = System::Drawing::Size(46, 23);
			this->button36->TabIndex = 22;
			this->button36->Text = L"Zmieñ";
			this->button36->UseVisualStyleBackColor = true;
			// 
			// textBox35
			// 
			this->textBox35->Location = System::Drawing::Point(101, 136);
			this->textBox35->Name = L"textBox35";
			this->textBox35->Size = System::Drawing::Size(166, 20);
			this->textBox35->TabIndex = 21;
			// 
			// label110
			// 
			this->label110->AutoSize = true;
			this->label110->Location = System::Drawing::Point(3, 139);
			this->label110->Name = L"label110";
			this->label110->Size = System::Drawing::Size(18, 13);
			this->label110->TabIndex = 20;
			this->label110->Text = L"N:";
			// 
			// button37
			// 
			this->button37->Location = System::Drawing::Point(273, 108);
			this->button37->Name = L"button37";
			this->button37->Size = System::Drawing::Size(46, 23);
			this->button37->TabIndex = 19;
			this->button37->Text = L"Zmieñ";
			this->button37->UseVisualStyleBackColor = true;
			// 
			// textBox36
			// 
			this->textBox36->Location = System::Drawing::Point(101, 110);
			this->textBox36->Name = L"textBox36";
			this->textBox36->Size = System::Drawing::Size(166, 20);
			this->textBox36->TabIndex = 18;
			// 
			// label111
			// 
			this->label111->AutoSize = true;
			this->label111->Location = System::Drawing::Point(3, 113);
			this->label111->Name = L"label111";
			this->label111->Size = System::Drawing::Size(87, 13);
			this->label111->TabIndex = 17;
			this->label111->Text = L"WP ca³kowania:";
			// 
			// button38
			// 
			this->button38->Location = System::Drawing::Point(273, 82);
			this->button38->Name = L"button38";
			this->button38->Size = System::Drawing::Size(46, 23);
			this->button38->TabIndex = 16;
			this->button38->Text = L"Zmieñ";
			this->button38->UseVisualStyleBackColor = true;
			// 
			// textBox37
			// 
			this->textBox37->Location = System::Drawing::Point(101, 84);
			this->textBox37->Name = L"textBox37";
			this->textBox37->Size = System::Drawing::Size(166, 20);
			this->textBox37->TabIndex = 15;
			// 
			// label112
			// 
			this->label112->AutoSize = true;
			this->label112->Location = System::Drawing::Point(3, 87);
			this->label112->Name = L"label112";
			this->label112->Size = System::Drawing::Size(23, 13);
			this->label112->TabIndex = 14;
			this->label112->Text = L"Tp:";
			// 
			// button39
			// 
			this->button39->Location = System::Drawing::Point(273, 56);
			this->button39->Name = L"button39";
			this->button39->Size = System::Drawing::Size(46, 23);
			this->button39->TabIndex = 13;
			this->button39->Text = L"Zmieñ";
			this->button39->UseVisualStyleBackColor = true;
			// 
			// textBox38
			// 
			this->textBox38->Location = System::Drawing::Point(101, 58);
			this->textBox38->Name = L"textBox38";
			this->textBox38->Size = System::Drawing::Size(166, 20);
			this->textBox38->TabIndex = 12;
			// 
			// label113
			// 
			this->label113->AutoSize = true;
			this->label113->Location = System::Drawing::Point(3, 61);
			this->label113->Name = L"label113";
			this->label113->Size = System::Drawing::Size(23, 13);
			this->label113->TabIndex = 11;
			this->label113->Text = L"Td:";
			// 
			// button40
			// 
			this->button40->Location = System::Drawing::Point(273, 30);
			this->button40->Name = L"button40";
			this->button40->Size = System::Drawing::Size(46, 23);
			this->button40->TabIndex = 10;
			this->button40->Text = L"Zmieñ";
			this->button40->UseVisualStyleBackColor = true;
			// 
			// textBox39
			// 
			this->textBox39->Location = System::Drawing::Point(101, 32);
			this->textBox39->Name = L"textBox39";
			this->textBox39->Size = System::Drawing::Size(166, 20);
			this->textBox39->TabIndex = 9;
			// 
			// label114
			// 
			this->label114->AutoSize = true;
			this->label114->Location = System::Drawing::Point(3, 35);
			this->label114->Name = L"label114";
			this->label114->Size = System::Drawing::Size(19, 13);
			this->label114->TabIndex = 8;
			this->label114->Text = L"Ti:";
			// 
			// button41
			// 
			this->button41->Location = System::Drawing::Point(273, 4);
			this->button41->Name = L"button41";
			this->button41->Size = System::Drawing::Size(46, 23);
			this->button41->TabIndex = 7;
			this->button41->Text = L"Zmieñ";
			this->button41->UseVisualStyleBackColor = true;
			// 
			// textBox40
			// 
			this->textBox40->Location = System::Drawing::Point(101, 6);
			this->textBox40->Name = L"textBox40";
			this->textBox40->Size = System::Drawing::Size(166, 20);
			this->textBox40->TabIndex = 6;
			// 
			// label115
			// 
			this->label115->AutoSize = true;
			this->label115->Location = System::Drawing::Point(3, 9);
			this->label115->Name = L"label115";
			this->label115->Size = System::Drawing::Size(16, 13);
			this->label115->TabIndex = 5;
			this->label115->Text = L"k:";
			// 
			// tabPage39
			// 
			this->tabPage39->Controls->Add(this->button42);
			this->tabPage39->Controls->Add(this->groupBox14);
			this->tabPage39->Controls->Add(this->tabControl11);
			this->tabPage39->Location = System::Drawing::Point(4, 22);
			this->tabPage39->Name = L"tabPage39";
			this->tabPage39->Padding = System::Windows::Forms::Padding(3);
			this->tabPage39->Size = System::Drawing::Size(325, 282);
			this->tabPage39->TabIndex = 1;
			this->tabPage39->Text = L"Zmiana wymuszenia";
			this->tabPage39->UseVisualStyleBackColor = true;
			// 
			// button42
			// 
			this->button42->Location = System::Drawing::Point(244, 253);
			this->button42->Name = L"button42";
			this->button42->Size = System::Drawing::Size(75, 23);
			this->button42->TabIndex = 9;
			this->button42->Text = L"Zmieñ";
			this->button42->UseVisualStyleBackColor = true;
			// 
			// groupBox14
			// 
			this->groupBox14->Controls->Add(this->checkBox7);
			this->groupBox14->Controls->Add(this->checkBox8);
			this->groupBox14->Controls->Add(this->checkBox9);
			this->groupBox14->Controls->Add(this->checkBox10);
			this->groupBox14->Controls->Add(this->checkBox11);
			this->groupBox14->Controls->Add(this->checkBox12);
			this->groupBox14->ForeColor = System::Drawing::Color::Black;
			this->groupBox14->Location = System::Drawing::Point(3, 8);
			this->groupBox14->Name = L"groupBox14";
			this->groupBox14->Size = System::Drawing::Size(316, 89);
			this->groupBox14->TabIndex = 7;
			this->groupBox14->TabStop = false;
			this->groupBox14->Text = L"Sk³adowe wymuszenia";
			// 
			// checkBox7
			// 
			this->checkBox7->AutoSize = true;
			this->checkBox7->Location = System::Drawing::Point(6, 19);
			this->checkBox7->Name = L"checkBox7";
			this->checkBox7->Size = System::Drawing::Size(51, 17);
			this->checkBox7->TabIndex = 1;
			this->checkBox7->Text = L"Skok";
			this->checkBox7->UseVisualStyleBackColor = true;
			// 
			// checkBox8
			// 
			this->checkBox8->AutoSize = true;
			this->checkBox8->Location = System::Drawing::Point(128, 65);
			this->checkBox8->Name = L"checkBox8";
			this->checkBox8->Size = System::Drawing::Size(52, 17);
			this->checkBox8->TabIndex = 6;
			this->checkBox8->Text = L"Szum";
			this->checkBox8->UseVisualStyleBackColor = true;
			// 
			// checkBox9
			// 
			this->checkBox9->AutoSize = true;
			this->checkBox9->Location = System::Drawing::Point(6, 42);
			this->checkBox9->Name = L"checkBox9";
			this->checkBox9->Size = System::Drawing::Size(108, 17);
			this->checkBox9->TabIndex = 2;
			this->checkBox9->Text = L"Impuls Kronekera";
			this->checkBox9->UseVisualStyleBackColor = true;
			// 
			// checkBox10
			// 
			this->checkBox10->AutoSize = true;
			this->checkBox10->Location = System::Drawing::Point(128, 42);
			this->checkBox10->Name = L"checkBox10";
			this->checkBox10->Size = System::Drawing::Size(59, 17);
			this->checkBox10->TabIndex = 5;
			this->checkBox10->Text = L"Trójk¹t";
			this->checkBox10->UseVisualStyleBackColor = true;
			// 
			// checkBox11
			// 
			this->checkBox11->AutoSize = true;
			this->checkBox11->Location = System::Drawing::Point(6, 65);
			this->checkBox11->Name = L"checkBox11";
			this->checkBox11->Size = System::Drawing::Size(71, 17);
			this->checkBox11->TabIndex = 3;
			this->checkBox11->Text = L"Prostok¹t";
			this->checkBox11->UseVisualStyleBackColor = true;
			// 
			// checkBox12
			// 
			this->checkBox12->AutoSize = true;
			this->checkBox12->Location = System::Drawing::Point(128, 19);
			this->checkBox12->Name = L"checkBox12";
			this->checkBox12->Size = System::Drawing::Size(52, 17);
			this->checkBox12->TabIndex = 4;
			this->checkBox12->Text = L"Sinus";
			this->checkBox12->UseVisualStyleBackColor = true;
			// 
			// tabControl11
			// 
			this->tabControl11->Controls->Add(this->tabPage40);
			this->tabControl11->Controls->Add(this->tabPage41);
			this->tabControl11->Controls->Add(this->tabPage42);
			this->tabControl11->Controls->Add(this->tabPage43);
			this->tabControl11->Controls->Add(this->tabPage44);
			this->tabControl11->Location = System::Drawing::Point(2, 103);
			this->tabControl11->Multiline = true;
			this->tabControl11->Name = L"tabControl11";
			this->tabControl11->SelectedIndex = 0;
			this->tabControl11->Size = System::Drawing::Size(316, 112);
			this->tabControl11->TabIndex = 8;
			// 
			// tabPage40
			// 
			this->tabPage40->Controls->Add(this->textBox41);
			this->tabPage40->Controls->Add(this->label116);
			this->tabPage40->Location = System::Drawing::Point(4, 22);
			this->tabPage40->Name = L"tabPage40";
			this->tabPage40->Padding = System::Windows::Forms::Padding(3);
			this->tabPage40->Size = System::Drawing::Size(308, 86);
			this->tabPage40->TabIndex = 0;
			this->tabPage40->Text = L"Skok";
			this->tabPage40->UseVisualStyleBackColor = true;
			// 
			// textBox41
			// 
			this->textBox41->Location = System::Drawing::Point(91, 6);
			this->textBox41->Name = L"textBox41";
			this->textBox41->Size = System::Drawing::Size(99, 20);
			this->textBox41->TabIndex = 1;
			// 
			// label116
			// 
			this->label116->AutoSize = true;
			this->label116->Location = System::Drawing::Point(6, 9);
			this->label116->Name = L"label116";
			this->label116->Size = System::Drawing::Size(79, 13);
			this->label116->TabIndex = 0;
			this->label116->Text = L"wartoœæ skoku:";
			// 
			// tabPage41
			// 
			this->tabPage41->Controls->Add(this->textBox42);
			this->tabPage41->Controls->Add(this->label117);
			this->tabPage41->Controls->Add(this->textBox43);
			this->tabPage41->Controls->Add(this->label118);
			this->tabPage41->Controls->Add(this->textBox44);
			this->tabPage41->Controls->Add(this->label119);
			this->tabPage41->Location = System::Drawing::Point(4, 22);
			this->tabPage41->Name = L"tabPage41";
			this->tabPage41->Padding = System::Windows::Forms::Padding(3);
			this->tabPage41->Size = System::Drawing::Size(308, 86);
			this->tabPage41->TabIndex = 1;
			this->tabPage41->Text = L"Prostok¹t";
			this->tabPage41->UseVisualStyleBackColor = true;
			// 
			// textBox42
			// 
			this->textBox42->Location = System::Drawing::Point(79, 58);
			this->textBox42->Name = L"textBox42";
			this->textBox42->Size = System::Drawing::Size(111, 20);
			this->textBox42->TabIndex = 7;
			// 
			// label117
			// 
			this->label117->AutoSize = true;
			this->label117->Location = System::Drawing::Point(6, 61);
			this->label117->Name = L"label117";
			this->label117->Size = System::Drawing::Size(67, 13);
			this->label117->TabIndex = 6;
			this->label117->Text = L"wype³nienie:";
			// 
			// textBox43
			// 
			this->textBox43->Location = System::Drawing::Point(79, 32);
			this->textBox43->Name = L"textBox43";
			this->textBox43->Size = System::Drawing::Size(111, 20);
			this->textBox43->TabIndex = 5;
			// 
			// label118
			// 
			this->label118->AutoSize = true;
			this->label118->Location = System::Drawing::Point(6, 35);
			this->label118->Name = L"label118";
			this->label118->Size = System::Drawing::Size(36, 13);
			this->label118->TabIndex = 4;
			this->label118->Text = L"okres:";
			// 
			// textBox44
			// 
			this->textBox44->Location = System::Drawing::Point(79, 6);
			this->textBox44->Name = L"textBox44";
			this->textBox44->Size = System::Drawing::Size(111, 20);
			this->textBox44->TabIndex = 3;
			// 
			// label119
			// 
			this->label119->AutoSize = true;
			this->label119->Location = System::Drawing::Point(6, 9);
			this->label119->Name = L"label119";
			this->label119->Size = System::Drawing::Size(55, 13);
			this->label119->TabIndex = 2;
			this->label119->Text = L"amplituda:";
			// 
			// tabPage42
			// 
			this->tabPage42->Controls->Add(this->textBox45);
			this->tabPage42->Controls->Add(this->label120);
			this->tabPage42->Controls->Add(this->textBox46);
			this->tabPage42->Controls->Add(this->label121);
			this->tabPage42->Location = System::Drawing::Point(4, 22);
			this->tabPage42->Name = L"tabPage42";
			this->tabPage42->Size = System::Drawing::Size(308, 86);
			this->tabPage42->TabIndex = 2;
			this->tabPage42->Text = L"Sinus";
			this->tabPage42->UseVisualStyleBackColor = true;
			// 
			// textBox45
			// 
			this->textBox45->Location = System::Drawing::Point(67, 32);
			this->textBox45->Name = L"textBox45";
			this->textBox45->Size = System::Drawing::Size(123, 20);
			this->textBox45->TabIndex = 9;
			// 
			// label120
			// 
			this->label120->AutoSize = true;
			this->label120->Location = System::Drawing::Point(6, 35);
			this->label120->Name = L"label120";
			this->label120->Size = System::Drawing::Size(36, 13);
			this->label120->TabIndex = 8;
			this->label120->Text = L"okres:";
			// 
			// textBox46
			// 
			this->textBox46->Location = System::Drawing::Point(67, 6);
			this->textBox46->Name = L"textBox46";
			this->textBox46->Size = System::Drawing::Size(123, 20);
			this->textBox46->TabIndex = 7;
			// 
			// label121
			// 
			this->label121->AutoSize = true;
			this->label121->Location = System::Drawing::Point(6, 9);
			this->label121->Name = L"label121";
			this->label121->Size = System::Drawing::Size(55, 13);
			this->label121->TabIndex = 6;
			this->label121->Text = L"amplituda:";
			// 
			// tabPage43
			// 
			this->tabPage43->Controls->Add(this->textBox47);
			this->tabPage43->Controls->Add(this->label122);
			this->tabPage43->Controls->Add(this->textBox48);
			this->tabPage43->Controls->Add(this->label123);
			this->tabPage43->Location = System::Drawing::Point(4, 22);
			this->tabPage43->Name = L"tabPage43";
			this->tabPage43->Size = System::Drawing::Size(308, 86);
			this->tabPage43->TabIndex = 3;
			this->tabPage43->Text = L"Trójk¹t";
			this->tabPage43->UseVisualStyleBackColor = true;
			// 
			// textBox47
			// 
			this->textBox47->Location = System::Drawing::Point(67, 32);
			this->textBox47->Name = L"textBox47";
			this->textBox47->Size = System::Drawing::Size(123, 20);
			this->textBox47->TabIndex = 13;
			// 
			// label122
			// 
			this->label122->AutoSize = true;
			this->label122->Location = System::Drawing::Point(6, 35);
			this->label122->Name = L"label122";
			this->label122->Size = System::Drawing::Size(36, 13);
			this->label122->TabIndex = 12;
			this->label122->Text = L"okres:";
			// 
			// textBox48
			// 
			this->textBox48->Location = System::Drawing::Point(67, 7);
			this->textBox48->Name = L"textBox48";
			this->textBox48->Size = System::Drawing::Size(123, 20);
			this->textBox48->TabIndex = 11;
			// 
			// label123
			// 
			this->label123->AutoSize = true;
			this->label123->Location = System::Drawing::Point(6, 9);
			this->label123->Name = L"label123";
			this->label123->Size = System::Drawing::Size(55, 13);
			this->label123->TabIndex = 10;
			this->label123->Text = L"amplituda:";
			// 
			// tabPage44
			// 
			this->tabPage44->Controls->Add(this->textBox49);
			this->tabPage44->Controls->Add(this->label124);
			this->tabPage44->Controls->Add(this->textBox50);
			this->tabPage44->Controls->Add(this->label125);
			this->tabPage44->Location = System::Drawing::Point(4, 22);
			this->tabPage44->Name = L"tabPage44";
			this->tabPage44->Size = System::Drawing::Size(308, 86);
			this->tabPage44->TabIndex = 4;
			this->tabPage44->Text = L"Szum";
			this->tabPage44->UseVisualStyleBackColor = true;
			// 
			// textBox49
			// 
			this->textBox49->Location = System::Drawing::Point(82, 32);
			this->textBox49->Name = L"textBox49";
			this->textBox49->Size = System::Drawing::Size(108, 20);
			this->textBox49->TabIndex = 13;
			// 
			// label124
			// 
			this->label124->AutoSize = true;
			this->label124->Location = System::Drawing::Point(6, 35);
			this->label124->Name = L"label124";
			this->label124->Size = System::Drawing::Size(73, 13);
			this->label124->TabIndex = 12;
			this->label124->Text = L"liczba próbek:";
			// 
			// textBox50
			// 
			this->textBox50->Location = System::Drawing::Point(82, 7);
			this->textBox50->Name = L"textBox50";
			this->textBox50->Size = System::Drawing::Size(108, 20);
			this->textBox50->TabIndex = 11;
			// 
			// label125
			// 
			this->label125->AutoSize = true;
			this->label125->Location = System::Drawing::Point(6, 10);
			this->label125->Name = L"label125";
			this->label125->Size = System::Drawing::Size(55, 13);
			this->label125->TabIndex = 10;
			this->label125->Text = L"wariancja:";
			// 
			// textBox51
			// 
			this->textBox51->Location = System::Drawing::Point(98, 431);
			this->textBox51->Name = L"textBox51";
			this->textBox51->Size = System::Drawing::Size(237, 20);
			this->textBox51->TabIndex = 19;
			// 
			// label126
			// 
			this->label126->AutoSize = true;
			this->label126->Location = System::Drawing::Point(5, 434);
			this->label126->Name = L"label126";
			this->label126->Size = System::Drawing::Size(27, 13);
			this->label126->TabIndex = 18;
			this->label126->Text = L"Plik:";
			// 
			// groupBox15
			// 
			this->groupBox15->Controls->Add(this->button43);
			this->groupBox15->Controls->Add(this->label127);
			this->groupBox15->Controls->Add(this->textBox52);
			this->groupBox15->ForeColor = System::Drawing::Color::Black;
			this->groupBox15->Location = System::Drawing::Point(6, 6);
			this->groupBox15->Name = L"groupBox15";
			this->groupBox15->Size = System::Drawing::Size(333, 79);
			this->groupBox15->TabIndex = 16;
			this->groupBox15->TabStop = false;
			this->groupBox15->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// button43
			// 
			this->button43->Location = System::Drawing::Point(252, 45);
			this->button43->Name = L"button43";
			this->button43->Size = System::Drawing::Size(75, 23);
			this->button43->TabIndex = 2;
			this->button43->Text = L"Wczytaj";
			this->button43->UseVisualStyleBackColor = true;
			// 
			// label127
			// 
			this->label127->AutoSize = true;
			this->label127->Location = System::Drawing::Point(6, 22);
			this->label127->Name = L"label127";
			this->label127->Size = System::Drawing::Size(93, 13);
			this->label127->TabIndex = 1;
			this->label127->Text = L"Nazwa regulatora:";
			// 
			// textBox52
			// 
			this->textBox52->Location = System::Drawing::Point(105, 19);
			this->textBox52->Name = L"textBox52";
			this->textBox52->Size = System::Drawing::Size(222, 20);
			this->textBox52->TabIndex = 0;
			// 
			// textBox53
			// 
			this->textBox53->Location = System::Drawing::Point(98, 405);
			this->textBox53->Name = L"textBox53";
			this->textBox53->Size = System::Drawing::Size(237, 20);
			this->textBox53->TabIndex = 17;
			// 
			// label128
			// 
			this->label128->AutoSize = true;
			this->label128->Location = System::Drawing::Point(4, 408);
			this->label128->Name = L"label128";
			this->label128->Size = System::Drawing::Size(93, 13);
			this->label128->TabIndex = 16;
			this->label128->Text = L"Nazwa regulatora:";
			// 
			// button44
			// 
			this->button44->Location = System::Drawing::Point(249, 457);
			this->button44->Name = L"button44";
			this->button44->Size = System::Drawing::Size(90, 23);
			this->button44->TabIndex = 15;
			this->button44->Text = L"Zapisz";
			this->button44->UseVisualStyleBackColor = true;
			// 
			// tabPage45
			// 
			this->tabPage45->Controls->Add(this->textBox54);
			this->tabPage45->Controls->Add(this->label129);
			this->tabPage45->Controls->Add(this->textBox55);
			this->tabPage45->Controls->Add(this->label130);
			this->tabPage45->Controls->Add(this->button45);
			this->tabPage45->Controls->Add(this->tabControl12);
			this->tabPage45->Controls->Add(this->groupBox17);
			this->tabPage45->Location = System::Drawing::Point(4, 22);
			this->tabPage45->Name = L"tabPage45";
			this->tabPage45->Size = System::Drawing::Size(345, 494);
			this->tabPage45->TabIndex = 3;
			this->tabPage45->Text = L"Regulator GPC";
			this->tabPage45->UseVisualStyleBackColor = true;
			// 
			// textBox54
			// 
			this->textBox54->Location = System::Drawing::Point(98, 431);
			this->textBox54->Name = L"textBox54";
			this->textBox54->Size = System::Drawing::Size(237, 20);
			this->textBox54->TabIndex = 26;
			// 
			// label129
			// 
			this->label129->AutoSize = true;
			this->label129->Location = System::Drawing::Point(5, 434);
			this->label129->Name = L"label129";
			this->label129->Size = System::Drawing::Size(27, 13);
			this->label129->TabIndex = 25;
			this->label129->Text = L"Plik:";
			// 
			// textBox55
			// 
			this->textBox55->Location = System::Drawing::Point(98, 405);
			this->textBox55->Name = L"textBox55";
			this->textBox55->Size = System::Drawing::Size(237, 20);
			this->textBox55->TabIndex = 24;
			// 
			// label130
			// 
			this->label130->AutoSize = true;
			this->label130->Location = System::Drawing::Point(4, 408);
			this->label130->Name = L"label130";
			this->label130->Size = System::Drawing::Size(93, 13);
			this->label130->TabIndex = 23;
			this->label130->Text = L"Nazwa regulatora:";
			// 
			// button45
			// 
			this->button45->Location = System::Drawing::Point(249, 457);
			this->button45->Name = L"button45";
			this->button45->Size = System::Drawing::Size(90, 23);
			this->button45->TabIndex = 22;
			this->button45->Text = L"Zapisz";
			this->button45->UseVisualStyleBackColor = true;
			// 
			// tabControl12
			// 
			this->tabControl12->Controls->Add(this->tabPage46);
			this->tabControl12->Controls->Add(this->tabPage47);
			this->tabControl12->Controls->Add(this->tabPage53);
			this->tabControl12->Location = System::Drawing::Point(6, 91);
			this->tabControl12->Name = L"tabControl12";
			this->tabControl12->SelectedIndex = 0;
			this->tabControl12->Size = System::Drawing::Size(333, 308);
			this->tabControl12->TabIndex = 21;
			// 
			// tabPage46
			// 
			this->tabPage46->Controls->Add(this->button46);
			this->tabPage46->Controls->Add(this->textBox56);
			this->tabPage46->Controls->Add(this->label131);
			this->tabPage46->Controls->Add(this->button47);
			this->tabPage46->Controls->Add(this->textBox57);
			this->tabPage46->Controls->Add(this->label132);
			this->tabPage46->Controls->Add(this->button48);
			this->tabPage46->Controls->Add(this->textBox58);
			this->tabPage46->Controls->Add(this->label133);
			this->tabPage46->Controls->Add(this->button49);
			this->tabPage46->Controls->Add(this->textBox59);
			this->tabPage46->Controls->Add(this->label134);
			this->tabPage46->Controls->Add(this->button50);
			this->tabPage46->Controls->Add(this->button51);
			this->tabPage46->Controls->Add(this->textBox60);
			this->tabPage46->Controls->Add(this->label135);
			this->tabPage46->Controls->Add(this->button52);
			this->tabPage46->Controls->Add(this->textBox61);
			this->tabPage46->Controls->Add(this->label136);
			this->tabPage46->Controls->Add(this->button53);
			this->tabPage46->Controls->Add(this->textBox62);
			this->tabPage46->Controls->Add(this->label137);
			this->tabPage46->Controls->Add(this->button54);
			this->tabPage46->Controls->Add(this->textBox63);
			this->tabPage46->Controls->Add(this->label138);
			this->tabPage46->Location = System::Drawing::Point(4, 22);
			this->tabPage46->Name = L"tabPage46";
			this->tabPage46->Padding = System::Windows::Forms::Padding(3);
			this->tabPage46->Size = System::Drawing::Size(325, 282);
			this->tabPage46->TabIndex = 0;
			this->tabPage46->Text = L"Zmiana parametrów";
			this->tabPage46->UseVisualStyleBackColor = true;
			// 
			// button46
			// 
			this->button46->Location = System::Drawing::Point(274, 133);
			this->button46->Name = L"button46";
			this->button46->Size = System::Drawing::Size(46, 23);
			this->button46->TabIndex = 49;
			this->button46->Text = L"Zmieñ";
			this->button46->UseVisualStyleBackColor = true;
			// 
			// textBox56
			// 
			this->textBox56->Location = System::Drawing::Point(102, 135);
			this->textBox56->Name = L"textBox56";
			this->textBox56->Size = System::Drawing::Size(166, 20);
			this->textBox56->TabIndex = 48;
			// 
			// label131
			// 
			this->label131->AutoSize = true;
			this->label131->Location = System::Drawing::Point(4, 138);
			this->label131->Name = L"label131";
			this->label131->Size = System::Drawing::Size(20, 13);
			this->label131->TabIndex = 47;
			this->label131->Text = L"dB";
			// 
			// button47
			// 
			this->button47->Location = System::Drawing::Point(274, 107);
			this->button47->Name = L"button47";
			this->button47->Size = System::Drawing::Size(46, 23);
			this->button47->TabIndex = 46;
			this->button47->Text = L"Zmieñ";
			this->button47->UseVisualStyleBackColor = true;
			// 
			// textBox57
			// 
			this->textBox57->Location = System::Drawing::Point(102, 109);
			this->textBox57->Name = L"textBox57";
			this->textBox57->Size = System::Drawing::Size(166, 20);
			this->textBox57->TabIndex = 45;
			// 
			// label132
			// 
			this->label132->AutoSize = true;
			this->label132->Location = System::Drawing::Point(4, 112);
			this->label132->Name = L"label132";
			this->label132->Size = System::Drawing::Size(20, 13);
			this->label132->TabIndex = 44;
			this->label132->Text = L"dA";
			// 
			// button48
			// 
			this->button48->Location = System::Drawing::Point(273, 184);
			this->button48->Name = L"button48";
			this->button48->Size = System::Drawing::Size(46, 23);
			this->button48->TabIndex = 43;
			this->button48->Text = L"Zmieñ";
			this->button48->UseVisualStyleBackColor = true;
			// 
			// textBox58
			// 
			this->textBox58->Location = System::Drawing::Point(101, 186);
			this->textBox58->Name = L"textBox58";
			this->textBox58->Size = System::Drawing::Size(166, 20);
			this->textBox58->TabIndex = 42;
			// 
			// label133
			// 
			this->label133->AutoSize = true;
			this->label133->Location = System::Drawing::Point(3, 189);
			this->label133->Name = L"label133";
			this->label133->Size = System::Drawing::Size(49, 13);
			this->label133->TabIndex = 41;
			this->label133->Text = L"Wy_Min:";
			// 
			// button49
			// 
			this->button49->Location = System::Drawing::Point(273, 158);
			this->button49->Name = L"button49";
			this->button49->Size = System::Drawing::Size(46, 23);
			this->button49->TabIndex = 40;
			this->button49->Text = L"Zmieñ";
			this->button49->UseVisualStyleBackColor = true;
			// 
			// textBox59
			// 
			this->textBox59->Location = System::Drawing::Point(101, 160);
			this->textBox59->Name = L"textBox59";
			this->textBox59->Size = System::Drawing::Size(166, 20);
			this->textBox59->TabIndex = 39;
			// 
			// label134
			// 
			this->label134->AutoSize = true;
			this->label134->Location = System::Drawing::Point(3, 163);
			this->label134->Name = L"label134";
			this->label134->Size = System::Drawing::Size(52, 13);
			this->label134->TabIndex = 38;
			this->label134->Text = L"Wy_Max:";
			// 
			// button50
			// 
			this->button50->Location = System::Drawing::Point(196, 253);
			this->button50->Name = L"button50";
			this->button50->Size = System::Drawing::Size(123, 23);
			this->button50->TabIndex = 37;
			this->button50->Text = L"Zmieñ wszystko";
			this->button50->UseVisualStyleBackColor = true;
			// 
			// button51
			// 
			this->button51->Location = System::Drawing::Point(273, 82);
			this->button51->Name = L"button51";
			this->button51->Size = System::Drawing::Size(46, 23);
			this->button51->TabIndex = 16;
			this->button51->Text = L"Zmieñ";
			this->button51->UseVisualStyleBackColor = true;
			// 
			// textBox60
			// 
			this->textBox60->Location = System::Drawing::Point(101, 84);
			this->textBox60->Name = L"textBox60";
			this->textBox60->Size = System::Drawing::Size(166, 20);
			this->textBox60->TabIndex = 15;
			// 
			// label135
			// 
			this->label135->AutoSize = true;
			this->label135->Location = System::Drawing::Point(3, 87);
			this->label135->Name = L"label135";
			this->label135->Size = System::Drawing::Size(27, 13);
			this->label135->TabIndex = 14;
			this->label135->Text = L"alfa:";
			// 
			// button52
			// 
			this->button52->Location = System::Drawing::Point(273, 56);
			this->button52->Name = L"button52";
			this->button52->Size = System::Drawing::Size(46, 23);
			this->button52->TabIndex = 13;
			this->button52->Text = L"Zmieñ";
			this->button52->UseVisualStyleBackColor = true;
			// 
			// textBox61
			// 
			this->textBox61->Location = System::Drawing::Point(101, 58);
			this->textBox61->Name = L"textBox61";
			this->textBox61->Size = System::Drawing::Size(166, 20);
			this->textBox61->TabIndex = 12;
			// 
			// label136
			// 
			this->label136->AutoSize = true;
			this->label136->Location = System::Drawing::Point(3, 61);
			this->label136->Name = L"label136";
			this->label136->Size = System::Drawing::Size(16, 13);
			this->label136->TabIndex = 11;
			this->label136->Text = L"?:";
			// 
			// button53
			// 
			this->button53->Location = System::Drawing::Point(273, 30);
			this->button53->Name = L"button53";
			this->button53->Size = System::Drawing::Size(46, 23);
			this->button53->TabIndex = 10;
			this->button53->Text = L"Zmieñ";
			this->button53->UseVisualStyleBackColor = true;
			// 
			// textBox62
			// 
			this->textBox62->Location = System::Drawing::Point(101, 32);
			this->textBox62->Name = L"textBox62";
			this->textBox62->Size = System::Drawing::Size(166, 20);
			this->textBox62->TabIndex = 9;
			// 
			// label137
			// 
			this->label137->AutoSize = true;
			this->label137->Location = System::Drawing::Point(3, 35);
			this->label137->Name = L"label137";
			this->label137->Size = System::Drawing::Size(16, 13);
			this->label137->TabIndex = 8;
			this->label137->Text = L"L:";
			// 
			// button54
			// 
			this->button54->Location = System::Drawing::Point(273, 4);
			this->button54->Name = L"button54";
			this->button54->Size = System::Drawing::Size(46, 23);
			this->button54->TabIndex = 7;
			this->button54->Text = L"Zmieñ";
			this->button54->UseVisualStyleBackColor = true;
			// 
			// textBox63
			// 
			this->textBox63->Location = System::Drawing::Point(101, 6);
			this->textBox63->Name = L"textBox63";
			this->textBox63->Size = System::Drawing::Size(166, 20);
			this->textBox63->TabIndex = 6;
			// 
			// label138
			// 
			this->label138->AutoSize = true;
			this->label138->Location = System::Drawing::Point(3, 9);
			this->label138->Name = L"label138";
			this->label138->Size = System::Drawing::Size(18, 13);
			this->label138->TabIndex = 5;
			this->label138->Text = L"H:";
			// 
			// tabPage47
			// 
			this->tabPage47->Controls->Add(this->button55);
			this->tabPage47->Controls->Add(this->groupBox16);
			this->tabPage47->Controls->Add(this->tabControl13);
			this->tabPage47->Location = System::Drawing::Point(4, 22);
			this->tabPage47->Name = L"tabPage47";
			this->tabPage47->Padding = System::Windows::Forms::Padding(3);
			this->tabPage47->Size = System::Drawing::Size(325, 282);
			this->tabPage47->TabIndex = 1;
			this->tabPage47->Text = L"Zmiana wymuszenia";
			this->tabPage47->UseVisualStyleBackColor = true;
			// 
			// button55
			// 
			this->button55->Location = System::Drawing::Point(244, 253);
			this->button55->Name = L"button55";
			this->button55->Size = System::Drawing::Size(75, 23);
			this->button55->TabIndex = 9;
			this->button55->Text = L"Zmieñ";
			this->button55->UseVisualStyleBackColor = true;
			// 
			// groupBox16
			// 
			this->groupBox16->Controls->Add(this->checkBox13);
			this->groupBox16->Controls->Add(this->checkBox14);
			this->groupBox16->Controls->Add(this->checkBox15);
			this->groupBox16->Controls->Add(this->checkBox16);
			this->groupBox16->Controls->Add(this->checkBox17);
			this->groupBox16->Controls->Add(this->checkBox18);
			this->groupBox16->ForeColor = System::Drawing::Color::Black;
			this->groupBox16->Location = System::Drawing::Point(3, 8);
			this->groupBox16->Name = L"groupBox16";
			this->groupBox16->Size = System::Drawing::Size(316, 89);
			this->groupBox16->TabIndex = 7;
			this->groupBox16->TabStop = false;
			this->groupBox16->Text = L"Sk³adowe wymuszenia";
			// 
			// checkBox13
			// 
			this->checkBox13->AutoSize = true;
			this->checkBox13->Location = System::Drawing::Point(6, 19);
			this->checkBox13->Name = L"checkBox13";
			this->checkBox13->Size = System::Drawing::Size(51, 17);
			this->checkBox13->TabIndex = 1;
			this->checkBox13->Text = L"Skok";
			this->checkBox13->UseVisualStyleBackColor = true;
			// 
			// checkBox14
			// 
			this->checkBox14->AutoSize = true;
			this->checkBox14->Location = System::Drawing::Point(128, 65);
			this->checkBox14->Name = L"checkBox14";
			this->checkBox14->Size = System::Drawing::Size(52, 17);
			this->checkBox14->TabIndex = 6;
			this->checkBox14->Text = L"Szum";
			this->checkBox14->UseVisualStyleBackColor = true;
			// 
			// checkBox15
			// 
			this->checkBox15->AutoSize = true;
			this->checkBox15->Location = System::Drawing::Point(6, 42);
			this->checkBox15->Name = L"checkBox15";
			this->checkBox15->Size = System::Drawing::Size(108, 17);
			this->checkBox15->TabIndex = 2;
			this->checkBox15->Text = L"Impuls Kronekera";
			this->checkBox15->UseVisualStyleBackColor = true;
			// 
			// checkBox16
			// 
			this->checkBox16->AutoSize = true;
			this->checkBox16->Location = System::Drawing::Point(128, 42);
			this->checkBox16->Name = L"checkBox16";
			this->checkBox16->Size = System::Drawing::Size(59, 17);
			this->checkBox16->TabIndex = 5;
			this->checkBox16->Text = L"Trójk¹t";
			this->checkBox16->UseVisualStyleBackColor = true;
			// 
			// checkBox17
			// 
			this->checkBox17->AutoSize = true;
			this->checkBox17->Location = System::Drawing::Point(6, 65);
			this->checkBox17->Name = L"checkBox17";
			this->checkBox17->Size = System::Drawing::Size(71, 17);
			this->checkBox17->TabIndex = 3;
			this->checkBox17->Text = L"Prostok¹t";
			this->checkBox17->UseVisualStyleBackColor = true;
			// 
			// checkBox18
			// 
			this->checkBox18->AutoSize = true;
			this->checkBox18->Location = System::Drawing::Point(128, 19);
			this->checkBox18->Name = L"checkBox18";
			this->checkBox18->Size = System::Drawing::Size(52, 17);
			this->checkBox18->TabIndex = 4;
			this->checkBox18->Text = L"Sinus";
			this->checkBox18->UseVisualStyleBackColor = true;
			// 
			// tabControl13
			// 
			this->tabControl13->Controls->Add(this->tabPage48);
			this->tabControl13->Controls->Add(this->tabPage49);
			this->tabControl13->Controls->Add(this->tabPage50);
			this->tabControl13->Controls->Add(this->tabPage51);
			this->tabControl13->Controls->Add(this->tabPage52);
			this->tabControl13->Location = System::Drawing::Point(2, 103);
			this->tabControl13->Multiline = true;
			this->tabControl13->Name = L"tabControl13";
			this->tabControl13->SelectedIndex = 0;
			this->tabControl13->Size = System::Drawing::Size(316, 112);
			this->tabControl13->TabIndex = 8;
			// 
			// tabPage48
			// 
			this->tabPage48->Controls->Add(this->textBox64);
			this->tabPage48->Controls->Add(this->label139);
			this->tabPage48->Location = System::Drawing::Point(4, 22);
			this->tabPage48->Name = L"tabPage48";
			this->tabPage48->Padding = System::Windows::Forms::Padding(3);
			this->tabPage48->Size = System::Drawing::Size(308, 86);
			this->tabPage48->TabIndex = 0;
			this->tabPage48->Text = L"Skok";
			this->tabPage48->UseVisualStyleBackColor = true;
			// 
			// textBox64
			// 
			this->textBox64->Location = System::Drawing::Point(91, 6);
			this->textBox64->Name = L"textBox64";
			this->textBox64->Size = System::Drawing::Size(99, 20);
			this->textBox64->TabIndex = 1;
			// 
			// label139
			// 
			this->label139->AutoSize = true;
			this->label139->Location = System::Drawing::Point(6, 9);
			this->label139->Name = L"label139";
			this->label139->Size = System::Drawing::Size(79, 13);
			this->label139->TabIndex = 0;
			this->label139->Text = L"wartoœæ skoku:";
			// 
			// tabPage49
			// 
			this->tabPage49->Controls->Add(this->textBox65);
			this->tabPage49->Controls->Add(this->label140);
			this->tabPage49->Controls->Add(this->textBox66);
			this->tabPage49->Controls->Add(this->label141);
			this->tabPage49->Controls->Add(this->textBox67);
			this->tabPage49->Controls->Add(this->label142);
			this->tabPage49->Location = System::Drawing::Point(4, 22);
			this->tabPage49->Name = L"tabPage49";
			this->tabPage49->Padding = System::Windows::Forms::Padding(3);
			this->tabPage49->Size = System::Drawing::Size(308, 86);
			this->tabPage49->TabIndex = 1;
			this->tabPage49->Text = L"Prostok¹t";
			this->tabPage49->UseVisualStyleBackColor = true;
			// 
			// textBox65
			// 
			this->textBox65->Location = System::Drawing::Point(79, 58);
			this->textBox65->Name = L"textBox65";
			this->textBox65->Size = System::Drawing::Size(111, 20);
			this->textBox65->TabIndex = 7;
			// 
			// label140
			// 
			this->label140->AutoSize = true;
			this->label140->Location = System::Drawing::Point(6, 61);
			this->label140->Name = L"label140";
			this->label140->Size = System::Drawing::Size(67, 13);
			this->label140->TabIndex = 6;
			this->label140->Text = L"wype³nienie:";
			// 
			// textBox66
			// 
			this->textBox66->Location = System::Drawing::Point(79, 32);
			this->textBox66->Name = L"textBox66";
			this->textBox66->Size = System::Drawing::Size(111, 20);
			this->textBox66->TabIndex = 5;
			// 
			// label141
			// 
			this->label141->AutoSize = true;
			this->label141->Location = System::Drawing::Point(6, 35);
			this->label141->Name = L"label141";
			this->label141->Size = System::Drawing::Size(36, 13);
			this->label141->TabIndex = 4;
			this->label141->Text = L"okres:";
			// 
			// textBox67
			// 
			this->textBox67->Location = System::Drawing::Point(79, 6);
			this->textBox67->Name = L"textBox67";
			this->textBox67->Size = System::Drawing::Size(111, 20);
			this->textBox67->TabIndex = 3;
			// 
			// label142
			// 
			this->label142->AutoSize = true;
			this->label142->Location = System::Drawing::Point(6, 9);
			this->label142->Name = L"label142";
			this->label142->Size = System::Drawing::Size(55, 13);
			this->label142->TabIndex = 2;
			this->label142->Text = L"amplituda:";
			// 
			// tabPage50
			// 
			this->tabPage50->Controls->Add(this->textBox68);
			this->tabPage50->Controls->Add(this->label143);
			this->tabPage50->Controls->Add(this->textBox69);
			this->tabPage50->Controls->Add(this->label144);
			this->tabPage50->Location = System::Drawing::Point(4, 22);
			this->tabPage50->Name = L"tabPage50";
			this->tabPage50->Size = System::Drawing::Size(308, 86);
			this->tabPage50->TabIndex = 2;
			this->tabPage50->Text = L"Sinus";
			this->tabPage50->UseVisualStyleBackColor = true;
			// 
			// textBox68
			// 
			this->textBox68->Location = System::Drawing::Point(67, 32);
			this->textBox68->Name = L"textBox68";
			this->textBox68->Size = System::Drawing::Size(123, 20);
			this->textBox68->TabIndex = 9;
			// 
			// label143
			// 
			this->label143->AutoSize = true;
			this->label143->Location = System::Drawing::Point(6, 35);
			this->label143->Name = L"label143";
			this->label143->Size = System::Drawing::Size(36, 13);
			this->label143->TabIndex = 8;
			this->label143->Text = L"okres:";
			// 
			// textBox69
			// 
			this->textBox69->Location = System::Drawing::Point(67, 6);
			this->textBox69->Name = L"textBox69";
			this->textBox69->Size = System::Drawing::Size(123, 20);
			this->textBox69->TabIndex = 7;
			// 
			// label144
			// 
			this->label144->AutoSize = true;
			this->label144->Location = System::Drawing::Point(6, 9);
			this->label144->Name = L"label144";
			this->label144->Size = System::Drawing::Size(55, 13);
			this->label144->TabIndex = 6;
			this->label144->Text = L"amplituda:";
			// 
			// tabPage51
			// 
			this->tabPage51->Controls->Add(this->textBox70);
			this->tabPage51->Controls->Add(this->label145);
			this->tabPage51->Controls->Add(this->textBox71);
			this->tabPage51->Controls->Add(this->label146);
			this->tabPage51->Location = System::Drawing::Point(4, 22);
			this->tabPage51->Name = L"tabPage51";
			this->tabPage51->Size = System::Drawing::Size(308, 86);
			this->tabPage51->TabIndex = 3;
			this->tabPage51->Text = L"Trójk¹t";
			this->tabPage51->UseVisualStyleBackColor = true;
			// 
			// textBox70
			// 
			this->textBox70->Location = System::Drawing::Point(67, 32);
			this->textBox70->Name = L"textBox70";
			this->textBox70->Size = System::Drawing::Size(123, 20);
			this->textBox70->TabIndex = 13;
			// 
			// label145
			// 
			this->label145->AutoSize = true;
			this->label145->Location = System::Drawing::Point(6, 35);
			this->label145->Name = L"label145";
			this->label145->Size = System::Drawing::Size(36, 13);
			this->label145->TabIndex = 12;
			this->label145->Text = L"okres:";
			// 
			// textBox71
			// 
			this->textBox71->Location = System::Drawing::Point(67, 7);
			this->textBox71->Name = L"textBox71";
			this->textBox71->Size = System::Drawing::Size(123, 20);
			this->textBox71->TabIndex = 11;
			// 
			// label146
			// 
			this->label146->AutoSize = true;
			this->label146->Location = System::Drawing::Point(6, 9);
			this->label146->Name = L"label146";
			this->label146->Size = System::Drawing::Size(55, 13);
			this->label146->TabIndex = 10;
			this->label146->Text = L"amplituda:";
			// 
			// tabPage52
			// 
			this->tabPage52->Controls->Add(this->textBox72);
			this->tabPage52->Controls->Add(this->label147);
			this->tabPage52->Controls->Add(this->textBox73);
			this->tabPage52->Controls->Add(this->label148);
			this->tabPage52->Location = System::Drawing::Point(4, 22);
			this->tabPage52->Name = L"tabPage52";
			this->tabPage52->Size = System::Drawing::Size(308, 86);
			this->tabPage52->TabIndex = 4;
			this->tabPage52->Text = L"Szum";
			this->tabPage52->UseVisualStyleBackColor = true;
			// 
			// textBox72
			// 
			this->textBox72->Location = System::Drawing::Point(82, 32);
			this->textBox72->Name = L"textBox72";
			this->textBox72->Size = System::Drawing::Size(108, 20);
			this->textBox72->TabIndex = 13;
			// 
			// label147
			// 
			this->label147->AutoSize = true;
			this->label147->Location = System::Drawing::Point(6, 35);
			this->label147->Name = L"label147";
			this->label147->Size = System::Drawing::Size(73, 13);
			this->label147->TabIndex = 12;
			this->label147->Text = L"liczba próbek:";
			// 
			// textBox73
			// 
			this->textBox73->Location = System::Drawing::Point(82, 7);
			this->textBox73->Name = L"textBox73";
			this->textBox73->Size = System::Drawing::Size(108, 20);
			this->textBox73->TabIndex = 11;
			// 
			// label148
			// 
			this->label148->AutoSize = true;
			this->label148->Location = System::Drawing::Point(6, 10);
			this->label148->Name = L"label148";
			this->label148->Size = System::Drawing::Size(55, 13);
			this->label148->TabIndex = 10;
			this->label148->Text = L"wariancja:";
			// 
			// tabPage53
			// 
			this->tabPage53->Controls->Add(this->button56);
			this->tabPage53->Controls->Add(this->button57);
			this->tabPage53->Controls->Add(this->label149);
			this->tabPage53->Controls->Add(this->textBox74);
			this->tabPage53->Controls->Add(this->textBox75);
			this->tabPage53->Location = System::Drawing::Point(4, 22);
			this->tabPage53->Name = L"tabPage53";
			this->tabPage53->Size = System::Drawing::Size(325, 282);
			this->tabPage53->TabIndex = 2;
			this->tabPage53->Text = L"Identyfikacja";
			this->tabPage53->UseVisualStyleBackColor = true;
			// 
			// button56
			// 
			this->button56->Location = System::Drawing::Point(247, 255);
			this->button56->Name = L"button56";
			this->button56->Size = System::Drawing::Size(75, 23);
			this->button56->TabIndex = 10;
			this->button56->Text = L"Wyczysc";
			this->button56->UseVisualStyleBackColor = true;
			// 
			// button57
			// 
			this->button57->Location = System::Drawing::Point(166, 5);
			this->button57->Name = L"button57";
			this->button57->Size = System::Drawing::Size(75, 23);
			this->button57->TabIndex = 9;
			this->button57->Text = L"Zmieñ";
			this->button57->UseVisualStyleBackColor = true;
			// 
			// label149
			// 
			this->label149->AutoSize = true;
			this->label149->Location = System::Drawing::Point(11, 10);
			this->label149->Name = L"label149";
			this->label149->Size = System::Drawing::Size(27, 13);
			this->label149->TabIndex = 8;
			this->label149->Text = L"alfa:";
			// 
			// textBox74
			// 
			this->textBox74->Location = System::Drawing::Point(3, 29);
			this->textBox74->Multiline = true;
			this->textBox74->Name = L"textBox74";
			this->textBox74->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox74->Size = System::Drawing::Size(319, 220);
			this->textBox74->TabIndex = 6;
			// 
			// textBox75
			// 
			this->textBox75->Location = System::Drawing::Point(58, 7);
			this->textBox75->Name = L"textBox75";
			this->textBox75->Size = System::Drawing::Size(102, 20);
			this->textBox75->TabIndex = 7;
			this->textBox75->Text = L"0.99";
			// 
			// groupBox17
			// 
			this->groupBox17->Controls->Add(this->button58);
			this->groupBox17->Controls->Add(this->label150);
			this->groupBox17->Controls->Add(this->textBox76);
			this->groupBox17->ForeColor = System::Drawing::Color::Black;
			this->groupBox17->Location = System::Drawing::Point(6, 6);
			this->groupBox17->Name = L"groupBox17";
			this->groupBox17->Size = System::Drawing::Size(333, 79);
			this->groupBox17->TabIndex = 17;
			this->groupBox17->TabStop = false;
			this->groupBox17->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// button58
			// 
			this->button58->Location = System::Drawing::Point(252, 45);
			this->button58->Name = L"button58";
			this->button58->Size = System::Drawing::Size(75, 23);
			this->button58->TabIndex = 2;
			this->button58->Text = L"Wczytaj";
			this->button58->UseVisualStyleBackColor = true;
			// 
			// label150
			// 
			this->label150->AutoSize = true;
			this->label150->Location = System::Drawing::Point(6, 22);
			this->label150->Name = L"label150";
			this->label150->Size = System::Drawing::Size(93, 13);
			this->label150->TabIndex = 1;
			this->label150->Text = L"Nazwa regulatora:";
			// 
			// textBox76
			// 
			this->textBox76->Location = System::Drawing::Point(105, 19);
			this->textBox76->Name = L"textBox76";
			this->textBox76->Size = System::Drawing::Size(222, 20);
			this->textBox76->TabIndex = 0;
			// 
			// tabPage54
			// 
			this->tabPage54->Controls->Add(this->button59);
			this->tabPage54->Controls->Add(this->label151);
			this->tabPage54->Controls->Add(this->textBox77);
			this->tabPage54->Controls->Add(this->button60);
			this->tabPage54->Controls->Add(this->textBox78);
			this->tabPage54->Controls->Add(this->checkBox19);
			this->tabPage54->Location = System::Drawing::Point(4, 22);
			this->tabPage54->Name = L"tabPage54";
			this->tabPage54->Size = System::Drawing::Size(345, 494);
			this->tabPage54->TabIndex = 4;
			this->tabPage54->Text = L"Identyfikacja";
			this->tabPage54->UseVisualStyleBackColor = true;
			// 
			// button59
			// 
			this->button59->Location = System::Drawing::Point(158, 24);
			this->button59->Name = L"button59";
			this->button59->Size = System::Drawing::Size(75, 23);
			this->button59->TabIndex = 5;
			this->button59->Text = L"Zmieñ";
			this->button59->UseVisualStyleBackColor = true;
			// 
			// label151
			// 
			this->label151->AutoSize = true;
			this->label151->Location = System::Drawing::Point(3, 29);
			this->label151->Name = L"label151";
			this->label151->Size = System::Drawing::Size(27, 13);
			this->label151->TabIndex = 4;
			this->label151->Text = L"alfa:";
			// 
			// textBox77
			// 
			this->textBox77->Location = System::Drawing::Point(50, 26);
			this->textBox77->Name = L"textBox77";
			this->textBox77->Size = System::Drawing::Size(102, 20);
			this->textBox77->TabIndex = 3;
			this->textBox77->Text = L"0.99";
			// 
			// button60
			// 
			this->button60->Location = System::Drawing::Point(267, 468);
			this->button60->Name = L"button60";
			this->button60->Size = System::Drawing::Size(75, 23);
			this->button60->TabIndex = 2;
			this->button60->Text = L"Wyczysc";
			this->button60->UseVisualStyleBackColor = true;
			// 
			// textBox78
			// 
			this->textBox78->Location = System::Drawing::Point(3, 52);
			this->textBox78->Multiline = true;
			this->textBox78->Name = L"textBox78";
			this->textBox78->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox78->Size = System::Drawing::Size(339, 410);
			this->textBox78->TabIndex = 1;
			// 
			// checkBox19
			// 
			this->checkBox19->AutoSize = true;
			this->checkBox19->Location = System::Drawing::Point(3, 3);
			this->checkBox19->Name = L"checkBox19";
			this->checkBox19->Size = System::Drawing::Size(147, 17);
			this->checkBox19->TabIndex = 0;
			this->checkBox19->Text = L"Identyfikuj obiekt ARMAX";
			this->checkBox19->UseVisualStyleBackColor = true;
			// 
			// tabPage55
			// 
			this->tabPage55->Controls->Add(this->groupBox18);
			this->tabPage55->Controls->Add(this->groupBox19);
			this->tabPage55->Location = System::Drawing::Point(4, 22);
			this->tabPage55->Name = L"tabPage55";
			this->tabPage55->Padding = System::Windows::Forms::Padding(3);
			this->tabPage55->Size = System::Drawing::Size(345, 494);
			this->tabPage55->TabIndex = 0;
			this->tabPage55->Text = L"Obiekt";
			this->tabPage55->UseVisualStyleBackColor = true;
			// 
			// groupBox18
			// 
			this->groupBox18->Controls->Add(this->button61);
			this->groupBox18->Controls->Add(this->textBox79);
			this->groupBox18->Controls->Add(this->label152);
			this->groupBox18->Controls->Add(this->button62);
			this->groupBox18->Controls->Add(this->button63);
			this->groupBox18->Controls->Add(this->textBox80);
			this->groupBox18->Controls->Add(this->button64);
			this->groupBox18->Controls->Add(this->textBox81);
			this->groupBox18->Controls->Add(this->label153);
			this->groupBox18->Controls->Add(this->textBox82);
			this->groupBox18->Controls->Add(this->label154);
			this->groupBox18->Controls->Add(this->button65);
			this->groupBox18->Controls->Add(this->label155);
			this->groupBox18->Controls->Add(this->textBox83);
			this->groupBox18->Controls->Add(this->button66);
			this->groupBox18->Controls->Add(this->label156);
			this->groupBox18->Controls->Add(this->textBox84);
			this->groupBox18->Controls->Add(this->label157);
			this->groupBox18->Controls->Add(this->button67);
			this->groupBox18->Controls->Add(this->label158);
			this->groupBox18->Controls->Add(this->button68);
			this->groupBox18->Controls->Add(this->button69);
			this->groupBox18->Controls->Add(this->textBox85);
			this->groupBox18->Controls->Add(this->textBox86);
			this->groupBox18->Controls->Add(this->button70);
			this->groupBox18->Controls->Add(this->textBox87);
			this->groupBox18->Controls->Add(this->button71);
			this->groupBox18->Controls->Add(this->textBox88);
			this->groupBox18->Controls->Add(this->label159);
			this->groupBox18->Controls->Add(this->label160);
			this->groupBox18->Controls->Add(this->label161);
			this->groupBox18->Controls->Add(this->button72);
			this->groupBox18->Controls->Add(this->textBox89);
			this->groupBox18->Controls->Add(this->button73);
			this->groupBox18->Controls->Add(this->textBox90);
			this->groupBox18->Controls->Add(this->button74);
			this->groupBox18->Controls->Add(this->textBox91);
			this->groupBox18->Controls->Add(this->label162);
			this->groupBox18->Controls->Add(this->label163);
			this->groupBox18->Controls->Add(this->label164);
			this->groupBox18->ForeColor = System::Drawing::Color::Black;
			this->groupBox18->Location = System::Drawing::Point(6, 6);
			this->groupBox18->Name = L"groupBox18";
			this->groupBox18->Size = System::Drawing::Size(333, 397);
			this->groupBox18->TabIndex = 9;
			this->groupBox18->TabStop = false;
			this->groupBox18->Text = L"Zmiana parametrów";
			// 
			// button61
			// 
			this->button61->Location = System::Drawing::Point(281, 327);
			this->button61->Name = L"button61";
			this->button61->Size = System::Drawing::Size(46, 23);
			this->button61->TabIndex = 39;
			this->button61->Text = L"Zmieñ";
			this->button61->UseVisualStyleBackColor = true;
			// 
			// textBox79
			// 
			this->textBox79->Location = System::Drawing::Point(117, 329);
			this->textBox79->Name = L"textBox79";
			this->textBox79->Size = System::Drawing::Size(158, 20);
			this->textBox79->TabIndex = 38;
			// 
			// label152
			// 
			this->label152->AutoSize = true;
			this->label152->Location = System::Drawing::Point(6, 332);
			this->label152->Name = L"label152";
			this->label152->Size = System::Drawing::Size(105, 13);
			this->label152->TabIndex = 37;
			this->label152->Text = L"d³ugoœæ zak³ócenia:";
			// 
			// button62
			// 
			this->button62->Location = System::Drawing::Point(204, 368);
			this->button62->Name = L"button62";
			this->button62->Size = System::Drawing::Size(123, 23);
			this->button62->TabIndex = 36;
			this->button62->Text = L"Zmieñ wszystko";
			this->button62->UseVisualStyleBackColor = true;
			// 
			// button63
			// 
			this->button63->Location = System::Drawing::Point(281, 223);
			this->button63->Name = L"button63";
			this->button63->Size = System::Drawing::Size(46, 23);
			this->button63->TabIndex = 35;
			this->button63->Text = L"Zmieñ";
			this->button63->UseVisualStyleBackColor = true;
			// 
			// textBox80
			// 
			this->textBox80->Location = System::Drawing::Point(117, 225);
			this->textBox80->Name = L"textBox80";
			this->textBox80->Size = System::Drawing::Size(158, 20);
			this->textBox80->TabIndex = 34;
			// 
			// button64
			// 
			this->button64->Location = System::Drawing::Point(281, 197);
			this->button64->Name = L"button64";
			this->button64->Size = System::Drawing::Size(46, 23);
			this->button64->TabIndex = 33;
			this->button64->Text = L"Zmieñ";
			this->button64->UseVisualStyleBackColor = true;
			// 
			// textBox81
			// 
			this->textBox81->Location = System::Drawing::Point(117, 251);
			this->textBox81->Name = L"textBox81";
			this->textBox81->Size = System::Drawing::Size(158, 20);
			this->textBox81->TabIndex = 21;
			// 
			// label153
			// 
			this->label153->AutoSize = true;
			this->label153->Location = System::Drawing::Point(7, 254);
			this->label153->Name = L"label153";
			this->label153->Size = System::Drawing::Size(40, 13);
			this->label153->TabIndex = 18;
			this->label153->Text = L"wyMin:";
			// 
			// textBox82
			// 
			this->textBox82->Location = System::Drawing::Point(117, 199);
			this->textBox82->Name = L"textBox82";
			this->textBox82->Size = System::Drawing::Size(158, 20);
			this->textBox82->TabIndex = 32;
			// 
			// label154
			// 
			this->label154->AutoSize = true;
			this->label154->Location = System::Drawing::Point(6, 280);
			this->label154->Name = L"label154";
			this->label154->Size = System::Drawing::Size(43, 13);
			this->label154->TabIndex = 19;
			this->label154->Text = L"wyMax:";
			// 
			// button65
			// 
			this->button65->Location = System::Drawing::Point(281, 171);
			this->button65->Name = L"button65";
			this->button65->Size = System::Drawing::Size(46, 23);
			this->button65->TabIndex = 31;
			this->button65->Text = L"Zmieñ";
			this->button65->UseVisualStyleBackColor = true;
			// 
			// label155
			// 
			this->label155->AutoSize = true;
			this->label155->Location = System::Drawing::Point(6, 306);
			this->label155->Name = L"label155";
			this->label155->Size = System::Drawing::Size(55, 13);
			this->label155->TabIndex = 20;
			this->label155->Text = L"wariancja:";
			// 
			// textBox83
			// 
			this->textBox83->Location = System::Drawing::Point(117, 173);
			this->textBox83->Name = L"textBox83";
			this->textBox83->Size = System::Drawing::Size(158, 20);
			this->textBox83->TabIndex = 30;
			// 
			// button66
			// 
			this->button66->Location = System::Drawing::Point(281, 249);
			this->button66->Name = L"button66";
			this->button66->Size = System::Drawing::Size(46, 23);
			this->button66->TabIndex = 22;
			this->button66->Text = L"Zmieñ";
			this->button66->UseVisualStyleBackColor = true;
			// 
			// label156
			// 
			this->label156->AutoSize = true;
			this->label156->Location = System::Drawing::Point(6, 228);
			this->label156->Name = L"label156";
			this->label156->Size = System::Drawing::Size(19, 13);
			this->label156->TabIndex = 29;
			this->label156->Text = L"I2:";
			// 
			// textBox84
			// 
			this->textBox84->Location = System::Drawing::Point(117, 277);
			this->textBox84->Name = L"textBox84";
			this->textBox84->Size = System::Drawing::Size(158, 20);
			this->textBox84->TabIndex = 23;
			// 
			// label157
			// 
			this->label157->AutoSize = true;
			this->label157->Location = System::Drawing::Point(6, 202);
			this->label157->Name = L"label157";
			this->label157->Size = System::Drawing::Size(19, 13);
			this->label157->TabIndex = 28;
			this->label157->Text = L"I1:";
			// 
			// button67
			// 
			this->button67->Location = System::Drawing::Point(281, 301);
			this->button67->Name = L"button67";
			this->button67->Size = System::Drawing::Size(46, 23);
			this->button67->TabIndex = 26;
			this->button67->Text = L"Zmieñ";
			this->button67->UseVisualStyleBackColor = true;
			// 
			// label158
			// 
			this->label158->AutoSize = true;
			this->label158->Location = System::Drawing::Point(7, 176);
			this->label158->Name = L"label158";
			this->label158->Size = System::Drawing::Size(16, 13);
			this->label158->TabIndex = 27;
			this->label158->Text = L"d:";
			// 
			// button68
			// 
			this->button68->Location = System::Drawing::Point(281, 275);
			this->button68->Name = L"button68";
			this->button68->Size = System::Drawing::Size(46, 23);
			this->button68->TabIndex = 24;
			this->button68->Text = L"Zmieñ";
			this->button68->UseVisualStyleBackColor = true;
			// 
			// button69
			// 
			this->button69->Location = System::Drawing::Point(281, 145);
			this->button69->Name = L"button69";
			this->button69->Size = System::Drawing::Size(46, 23);
			this->button69->TabIndex = 17;
			this->button69->Text = L"Zmieñ";
			this->button69->UseVisualStyleBackColor = true;
			// 
			// textBox85
			// 
			this->textBox85->Location = System::Drawing::Point(117, 303);
			this->textBox85->Name = L"textBox85";
			this->textBox85->Size = System::Drawing::Size(158, 20);
			this->textBox85->TabIndex = 25;
			// 
			// textBox86
			// 
			this->textBox86->Location = System::Drawing::Point(117, 147);
			this->textBox86->Name = L"textBox86";
			this->textBox86->Size = System::Drawing::Size(158, 20);
			this->textBox86->TabIndex = 16;
			// 
			// button70
			// 
			this->button70->Location = System::Drawing::Point(281, 119);
			this->button70->Name = L"button70";
			this->button70->Size = System::Drawing::Size(46, 23);
			this->button70->TabIndex = 15;
			this->button70->Text = L"Zmieñ";
			this->button70->UseVisualStyleBackColor = true;
			// 
			// textBox87
			// 
			this->textBox87->Location = System::Drawing::Point(117, 121);
			this->textBox87->Name = L"textBox87";
			this->textBox87->Size = System::Drawing::Size(158, 20);
			this->textBox87->TabIndex = 14;
			// 
			// button71
			// 
			this->button71->Location = System::Drawing::Point(281, 93);
			this->button71->Name = L"button71";
			this->button71->Size = System::Drawing::Size(46, 23);
			this->button71->TabIndex = 13;
			this->button71->Text = L"Zmieñ";
			this->button71->UseVisualStyleBackColor = true;
			// 
			// textBox88
			// 
			this->textBox88->Location = System::Drawing::Point(117, 95);
			this->textBox88->Name = L"textBox88";
			this->textBox88->Size = System::Drawing::Size(158, 20);
			this->textBox88->TabIndex = 12;
			// 
			// label159
			// 
			this->label159->AutoSize = true;
			this->label159->Location = System::Drawing::Point(6, 150);
			this->label159->Name = L"label159";
			this->label159->Size = System::Drawing::Size(23, 13);
			this->label159->TabIndex = 11;
			this->label159->Text = L"C2:";
			// 
			// label160
			// 
			this->label160->AutoSize = true;
			this->label160->Location = System::Drawing::Point(6, 124);
			this->label160->Name = L"label160";
			this->label160->Size = System::Drawing::Size(23, 13);
			this->label160->TabIndex = 10;
			this->label160->Text = L"B2:";
			// 
			// label161
			// 
			this->label161->AutoSize = true;
			this->label161->Location = System::Drawing::Point(7, 98);
			this->label161->Name = L"label161";
			this->label161->Size = System::Drawing::Size(23, 13);
			this->label161->TabIndex = 9;
			this->label161->Text = L"A2:";
			// 
			// button72
			// 
			this->button72->Location = System::Drawing::Point(281, 67);
			this->button72->Name = L"button72";
			this->button72->Size = System::Drawing::Size(46, 23);
			this->button72->TabIndex = 8;
			this->button72->Text = L"Zmieñ";
			this->button72->UseVisualStyleBackColor = true;
			// 
			// textBox89
			// 
			this->textBox89->Location = System::Drawing::Point(117, 69);
			this->textBox89->Name = L"textBox89";
			this->textBox89->Size = System::Drawing::Size(158, 20);
			this->textBox89->TabIndex = 7;
			// 
			// button73
			// 
			this->button73->Location = System::Drawing::Point(281, 41);
			this->button73->Name = L"button73";
			this->button73->Size = System::Drawing::Size(46, 23);
			this->button73->TabIndex = 6;
			this->button73->Text = L"Zmieñ";
			this->button73->UseVisualStyleBackColor = true;
			// 
			// textBox90
			// 
			this->textBox90->Location = System::Drawing::Point(117, 43);
			this->textBox90->Name = L"textBox90";
			this->textBox90->Size = System::Drawing::Size(158, 20);
			this->textBox90->TabIndex = 5;
			// 
			// button74
			// 
			this->button74->Location = System::Drawing::Point(281, 15);
			this->button74->Name = L"button74";
			this->button74->Size = System::Drawing::Size(46, 23);
			this->button74->TabIndex = 4;
			this->button74->Text = L"Zmieñ";
			this->button74->UseVisualStyleBackColor = true;
			// 
			// textBox91
			// 
			this->textBox91->Location = System::Drawing::Point(117, 13);
			this->textBox91->Name = L"textBox91";
			this->textBox91->Size = System::Drawing::Size(158, 20);
			this->textBox91->TabIndex = 3;
			// 
			// label162
			// 
			this->label162->AutoSize = true;
			this->label162->Location = System::Drawing::Point(6, 72);
			this->label162->Name = L"label162";
			this->label162->Size = System::Drawing::Size(23, 13);
			this->label162->TabIndex = 2;
			this->label162->Text = L"C1:";
			// 
			// label163
			// 
			this->label163->AutoSize = true;
			this->label163->Location = System::Drawing::Point(6, 46);
			this->label163->Name = L"label163";
			this->label163->Size = System::Drawing::Size(23, 13);
			this->label163->TabIndex = 1;
			this->label163->Text = L"B1:";
			// 
			// label164
			// 
			this->label164->AutoSize = true;
			this->label164->Location = System::Drawing::Point(7, 20);
			this->label164->Name = L"label164";
			this->label164->Size = System::Drawing::Size(23, 13);
			this->label164->TabIndex = 0;
			this->label164->Text = L"A1:";
			// 
			// groupBox19
			// 
			this->groupBox19->Controls->Add(this->button75);
			this->groupBox19->Controls->Add(this->label165);
			this->groupBox19->Controls->Add(this->textBox92);
			this->groupBox19->ForeColor = System::Drawing::Color::Black;
			this->groupBox19->Location = System::Drawing::Point(6, 409);
			this->groupBox19->Name = L"groupBox19";
			this->groupBox19->Size = System::Drawing::Size(333, 79);
			this->groupBox19->TabIndex = 10;
			this->groupBox19->TabStop = false;
			this->groupBox19->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// button75
			// 
			this->button75->Location = System::Drawing::Point(252, 45);
			this->button75->Name = L"button75";
			this->button75->Size = System::Drawing::Size(75, 23);
			this->button75->TabIndex = 2;
			this->button75->Text = L"Wczytaj";
			this->button75->UseVisualStyleBackColor = true;
			// 
			// label165
			// 
			this->label165->AutoSize = true;
			this->label165->Location = System::Drawing::Point(6, 22);
			this->label165->Name = L"label165";
			this->label165->Size = System::Drawing::Size(81, 13);
			this->label165->TabIndex = 1;
			this->label165->Text = L"Nazwa obiektu:";
			// 
			// textBox92
			// 
			this->textBox92->Location = System::Drawing::Point(93, 19);
			this->textBox92->Name = L"textBox92";
			this->textBox92->Size = System::Drawing::Size(234, 20);
			this->textBox92->TabIndex = 0;
			// 
			// tabPage56
			// 
			this->tabPage56->Controls->Add(this->tabControl14);
			this->tabPage56->Controls->Add(this->textBox106);
			this->tabPage56->Controls->Add(this->label179);
			this->tabPage56->Controls->Add(this->groupBox21);
			this->tabPage56->Controls->Add(this->textBox108);
			this->tabPage56->Controls->Add(this->label181);
			this->tabPage56->Controls->Add(this->button82);
			this->tabPage56->Location = System::Drawing::Point(4, 22);
			this->tabPage56->Name = L"tabPage56";
			this->tabPage56->Padding = System::Windows::Forms::Padding(3);
			this->tabPage56->Size = System::Drawing::Size(345, 494);
			this->tabPage56->TabIndex = 1;
			this->tabPage56->Text = L"Regulator P";
			this->tabPage56->UseVisualStyleBackColor = true;
			// 
			// tabControl14
			// 
			this->tabControl14->Controls->Add(this->tabPage57);
			this->tabControl14->Controls->Add(this->tabPage58);
			this->tabControl14->Location = System::Drawing::Point(6, 91);
			this->tabControl14->Name = L"tabControl14";
			this->tabControl14->SelectedIndex = 0;
			this->tabControl14->Size = System::Drawing::Size(333, 308);
			this->tabControl14->TabIndex = 12;
			// 
			// tabPage57
			// 
			this->tabPage57->Controls->Add(this->button76);
			this->tabPage57->Controls->Add(this->button77);
			this->tabPage57->Controls->Add(this->textBox93);
			this->tabPage57->Controls->Add(this->label166);
			this->tabPage57->Controls->Add(this->button78);
			this->tabPage57->Controls->Add(this->textBox94);
			this->tabPage57->Controls->Add(this->label167);
			this->tabPage57->Controls->Add(this->textBox95);
			this->tabPage57->Controls->Add(this->label168);
			this->tabPage57->Controls->Add(this->button79);
			this->tabPage57->Location = System::Drawing::Point(4, 22);
			this->tabPage57->Name = L"tabPage57";
			this->tabPage57->Padding = System::Windows::Forms::Padding(3);
			this->tabPage57->Size = System::Drawing::Size(325, 282);
			this->tabPage57->TabIndex = 0;
			this->tabPage57->Text = L"Zmiana parametrów";
			this->tabPage57->UseVisualStyleBackColor = true;
			// 
			// button76
			// 
			this->button76->Location = System::Drawing::Point(196, 253);
			this->button76->Name = L"button76";
			this->button76->Size = System::Drawing::Size(123, 23);
			this->button76->TabIndex = 50;
			this->button76->Text = L"Zmieñ wszystko";
			this->button76->UseVisualStyleBackColor = true;
			// 
			// button77
			// 
			this->button77->Location = System::Drawing::Point(273, 56);
			this->button77->Name = L"button77";
			this->button77->Size = System::Drawing::Size(46, 23);
			this->button77->TabIndex = 49;
			this->button77->Text = L"Zmieñ";
			this->button77->UseVisualStyleBackColor = true;
			// 
			// textBox93
			// 
			this->textBox93->Location = System::Drawing::Point(58, 58);
			this->textBox93->Name = L"textBox93";
			this->textBox93->Size = System::Drawing::Size(209, 20);
			this->textBox93->TabIndex = 48;
			// 
			// label166
			// 
			this->label166->AutoSize = true;
			this->label166->Location = System::Drawing::Point(3, 61);
			this->label166->Name = L"label166";
			this->label166->Size = System::Drawing::Size(49, 13);
			this->label166->TabIndex = 47;
			this->label166->Text = L"Wy_Min:";
			// 
			// button78
			// 
			this->button78->Location = System::Drawing::Point(273, 30);
			this->button78->Name = L"button78";
			this->button78->Size = System::Drawing::Size(46, 23);
			this->button78->TabIndex = 46;
			this->button78->Text = L"Zmieñ";
			this->button78->UseVisualStyleBackColor = true;
			// 
			// textBox94
			// 
			this->textBox94->Location = System::Drawing::Point(58, 32);
			this->textBox94->Name = L"textBox94";
			this->textBox94->Size = System::Drawing::Size(209, 20);
			this->textBox94->TabIndex = 45;
			// 
			// label167
			// 
			this->label167->AutoSize = true;
			this->label167->Location = System::Drawing::Point(3, 35);
			this->label167->Name = L"label167";
			this->label167->Size = System::Drawing::Size(52, 13);
			this->label167->TabIndex = 44;
			this->label167->Text = L"Wy_Max:";
			// 
			// textBox95
			// 
			this->textBox95->Location = System::Drawing::Point(58, 6);
			this->textBox95->Name = L"textBox95";
			this->textBox95->Size = System::Drawing::Size(209, 20);
			this->textBox95->TabIndex = 14;
			// 
			// label168
			// 
			this->label168->AutoSize = true;
			this->label168->Location = System::Drawing::Point(3, 9);
			this->label168->Name = L"label168";
			this->label168->Size = System::Drawing::Size(16, 13);
			this->label168->TabIndex = 13;
			this->label168->Text = L"k:";
			// 
			// button79
			// 
			this->button79->Location = System::Drawing::Point(273, 4);
			this->button79->Name = L"button79";
			this->button79->Size = System::Drawing::Size(46, 23);
			this->button79->TabIndex = 12;
			this->button79->Text = L"Zmieñ";
			this->button79->UseVisualStyleBackColor = true;
			// 
			// tabPage58
			// 
			this->tabPage58->Controls->Add(this->button80);
			this->tabPage58->Controls->Add(this->groupBox20);
			this->tabPage58->Controls->Add(this->tabControl15);
			this->tabPage58->Location = System::Drawing::Point(4, 22);
			this->tabPage58->Name = L"tabPage58";
			this->tabPage58->Padding = System::Windows::Forms::Padding(3);
			this->tabPage58->Size = System::Drawing::Size(325, 282);
			this->tabPage58->TabIndex = 1;
			this->tabPage58->Text = L"Zmiana wymuszenia";
			this->tabPage58->UseVisualStyleBackColor = true;
			// 
			// button80
			// 
			this->button80->Location = System::Drawing::Point(244, 253);
			this->button80->Name = L"button80";
			this->button80->Size = System::Drawing::Size(75, 23);
			this->button80->TabIndex = 9;
			this->button80->Text = L"Zmieñ";
			this->button80->UseVisualStyleBackColor = true;
			// 
			// groupBox20
			// 
			this->groupBox20->Controls->Add(this->checkBox20);
			this->groupBox20->Controls->Add(this->checkBox21);
			this->groupBox20->Controls->Add(this->checkBox22);
			this->groupBox20->Controls->Add(this->checkBox23);
			this->groupBox20->Controls->Add(this->checkBox24);
			this->groupBox20->Controls->Add(this->checkBox25);
			this->groupBox20->ForeColor = System::Drawing::Color::Black;
			this->groupBox20->Location = System::Drawing::Point(3, 8);
			this->groupBox20->Name = L"groupBox20";
			this->groupBox20->Size = System::Drawing::Size(316, 89);
			this->groupBox20->TabIndex = 7;
			this->groupBox20->TabStop = false;
			this->groupBox20->Text = L"Sk³adowe wymuszenia";
			// 
			// checkBox20
			// 
			this->checkBox20->AutoSize = true;
			this->checkBox20->Location = System::Drawing::Point(6, 19);
			this->checkBox20->Name = L"checkBox20";
			this->checkBox20->Size = System::Drawing::Size(51, 17);
			this->checkBox20->TabIndex = 1;
			this->checkBox20->Text = L"Skok";
			this->checkBox20->UseVisualStyleBackColor = true;
			// 
			// checkBox21
			// 
			this->checkBox21->AutoSize = true;
			this->checkBox21->Location = System::Drawing::Point(128, 65);
			this->checkBox21->Name = L"checkBox21";
			this->checkBox21->Size = System::Drawing::Size(52, 17);
			this->checkBox21->TabIndex = 6;
			this->checkBox21->Text = L"Szum";
			this->checkBox21->UseVisualStyleBackColor = true;
			// 
			// checkBox22
			// 
			this->checkBox22->AutoSize = true;
			this->checkBox22->Location = System::Drawing::Point(6, 42);
			this->checkBox22->Name = L"checkBox22";
			this->checkBox22->Size = System::Drawing::Size(108, 17);
			this->checkBox22->TabIndex = 2;
			this->checkBox22->Text = L"Impuls Kronekera";
			this->checkBox22->UseVisualStyleBackColor = true;
			// 
			// checkBox23
			// 
			this->checkBox23->AutoSize = true;
			this->checkBox23->Location = System::Drawing::Point(128, 42);
			this->checkBox23->Name = L"checkBox23";
			this->checkBox23->Size = System::Drawing::Size(59, 17);
			this->checkBox23->TabIndex = 5;
			this->checkBox23->Text = L"Trójk¹t";
			this->checkBox23->UseVisualStyleBackColor = true;
			// 
			// checkBox24
			// 
			this->checkBox24->AutoSize = true;
			this->checkBox24->Location = System::Drawing::Point(6, 65);
			this->checkBox24->Name = L"checkBox24";
			this->checkBox24->Size = System::Drawing::Size(71, 17);
			this->checkBox24->TabIndex = 3;
			this->checkBox24->Text = L"Prostok¹t";
			this->checkBox24->UseVisualStyleBackColor = true;
			// 
			// checkBox25
			// 
			this->checkBox25->AutoSize = true;
			this->checkBox25->Location = System::Drawing::Point(128, 19);
			this->checkBox25->Name = L"checkBox25";
			this->checkBox25->Size = System::Drawing::Size(52, 17);
			this->checkBox25->TabIndex = 4;
			this->checkBox25->Text = L"Sinus";
			this->checkBox25->UseVisualStyleBackColor = true;
			// 
			// tabControl15
			// 
			this->tabControl15->Controls->Add(this->tabPage59);
			this->tabControl15->Controls->Add(this->tabPage60);
			this->tabControl15->Controls->Add(this->tabPage61);
			this->tabControl15->Controls->Add(this->tabPage62);
			this->tabControl15->Controls->Add(this->tabPage63);
			this->tabControl15->Location = System::Drawing::Point(2, 103);
			this->tabControl15->Multiline = true;
			this->tabControl15->Name = L"tabControl15";
			this->tabControl15->SelectedIndex = 0;
			this->tabControl15->Size = System::Drawing::Size(317, 112);
			this->tabControl15->TabIndex = 8;
			// 
			// tabPage59
			// 
			this->tabPage59->Controls->Add(this->textBox96);
			this->tabPage59->Controls->Add(this->label169);
			this->tabPage59->Location = System::Drawing::Point(4, 22);
			this->tabPage59->Name = L"tabPage59";
			this->tabPage59->Padding = System::Windows::Forms::Padding(3);
			this->tabPage59->Size = System::Drawing::Size(309, 86);
			this->tabPage59->TabIndex = 0;
			this->tabPage59->Text = L"Skok";
			this->tabPage59->UseVisualStyleBackColor = true;
			// 
			// textBox96
			// 
			this->textBox96->Location = System::Drawing::Point(91, 6);
			this->textBox96->Name = L"textBox96";
			this->textBox96->Size = System::Drawing::Size(99, 20);
			this->textBox96->TabIndex = 1;
			// 
			// label169
			// 
			this->label169->AutoSize = true;
			this->label169->Location = System::Drawing::Point(6, 9);
			this->label169->Name = L"label169";
			this->label169->Size = System::Drawing::Size(79, 13);
			this->label169->TabIndex = 0;
			this->label169->Text = L"wartoœæ skoku:";
			// 
			// tabPage60
			// 
			this->tabPage60->Controls->Add(this->textBox97);
			this->tabPage60->Controls->Add(this->label170);
			this->tabPage60->Controls->Add(this->textBox98);
			this->tabPage60->Controls->Add(this->label171);
			this->tabPage60->Controls->Add(this->textBox99);
			this->tabPage60->Controls->Add(this->label172);
			this->tabPage60->Location = System::Drawing::Point(4, 22);
			this->tabPage60->Name = L"tabPage60";
			this->tabPage60->Padding = System::Windows::Forms::Padding(3);
			this->tabPage60->Size = System::Drawing::Size(309, 86);
			this->tabPage60->TabIndex = 1;
			this->tabPage60->Text = L"Prostok¹t";
			this->tabPage60->UseVisualStyleBackColor = true;
			// 
			// textBox97
			// 
			this->textBox97->Location = System::Drawing::Point(79, 58);
			this->textBox97->Name = L"textBox97";
			this->textBox97->Size = System::Drawing::Size(111, 20);
			this->textBox97->TabIndex = 7;
			// 
			// label170
			// 
			this->label170->AutoSize = true;
			this->label170->Location = System::Drawing::Point(6, 61);
			this->label170->Name = L"label170";
			this->label170->Size = System::Drawing::Size(67, 13);
			this->label170->TabIndex = 6;
			this->label170->Text = L"wype³nienie:";
			// 
			// textBox98
			// 
			this->textBox98->Location = System::Drawing::Point(79, 32);
			this->textBox98->Name = L"textBox98";
			this->textBox98->Size = System::Drawing::Size(111, 20);
			this->textBox98->TabIndex = 5;
			// 
			// label171
			// 
			this->label171->AutoSize = true;
			this->label171->Location = System::Drawing::Point(6, 35);
			this->label171->Name = L"label171";
			this->label171->Size = System::Drawing::Size(36, 13);
			this->label171->TabIndex = 4;
			this->label171->Text = L"okres:";
			// 
			// textBox99
			// 
			this->textBox99->Location = System::Drawing::Point(79, 6);
			this->textBox99->Name = L"textBox99";
			this->textBox99->Size = System::Drawing::Size(111, 20);
			this->textBox99->TabIndex = 3;
			// 
			// label172
			// 
			this->label172->AutoSize = true;
			this->label172->Location = System::Drawing::Point(6, 9);
			this->label172->Name = L"label172";
			this->label172->Size = System::Drawing::Size(55, 13);
			this->label172->TabIndex = 2;
			this->label172->Text = L"amplituda:";
			// 
			// tabPage61
			// 
			this->tabPage61->Controls->Add(this->textBox100);
			this->tabPage61->Controls->Add(this->label173);
			this->tabPage61->Controls->Add(this->textBox101);
			this->tabPage61->Controls->Add(this->label174);
			this->tabPage61->Location = System::Drawing::Point(4, 22);
			this->tabPage61->Name = L"tabPage61";
			this->tabPage61->Size = System::Drawing::Size(309, 86);
			this->tabPage61->TabIndex = 2;
			this->tabPage61->Text = L"Sinus";
			this->tabPage61->UseVisualStyleBackColor = true;
			// 
			// textBox100
			// 
			this->textBox100->Location = System::Drawing::Point(67, 32);
			this->textBox100->Name = L"textBox100";
			this->textBox100->Size = System::Drawing::Size(123, 20);
			this->textBox100->TabIndex = 9;
			// 
			// label173
			// 
			this->label173->AutoSize = true;
			this->label173->Location = System::Drawing::Point(6, 35);
			this->label173->Name = L"label173";
			this->label173->Size = System::Drawing::Size(36, 13);
			this->label173->TabIndex = 8;
			this->label173->Text = L"okres:";
			// 
			// textBox101
			// 
			this->textBox101->Location = System::Drawing::Point(67, 6);
			this->textBox101->Name = L"textBox101";
			this->textBox101->Size = System::Drawing::Size(123, 20);
			this->textBox101->TabIndex = 7;
			// 
			// label174
			// 
			this->label174->AutoSize = true;
			this->label174->Location = System::Drawing::Point(6, 9);
			this->label174->Name = L"label174";
			this->label174->Size = System::Drawing::Size(55, 13);
			this->label174->TabIndex = 6;
			this->label174->Text = L"amplituda:";
			// 
			// tabPage62
			// 
			this->tabPage62->Controls->Add(this->textBox102);
			this->tabPage62->Controls->Add(this->label175);
			this->tabPage62->Controls->Add(this->textBox103);
			this->tabPage62->Controls->Add(this->label176);
			this->tabPage62->Location = System::Drawing::Point(4, 22);
			this->tabPage62->Name = L"tabPage62";
			this->tabPage62->Size = System::Drawing::Size(309, 86);
			this->tabPage62->TabIndex = 3;
			this->tabPage62->Text = L"Trójk¹t";
			this->tabPage62->UseVisualStyleBackColor = true;
			// 
			// textBox102
			// 
			this->textBox102->Location = System::Drawing::Point(67, 32);
			this->textBox102->Name = L"textBox102";
			this->textBox102->Size = System::Drawing::Size(123, 20);
			this->textBox102->TabIndex = 13;
			// 
			// label175
			// 
			this->label175->AutoSize = true;
			this->label175->Location = System::Drawing::Point(6, 35);
			this->label175->Name = L"label175";
			this->label175->Size = System::Drawing::Size(36, 13);
			this->label175->TabIndex = 12;
			this->label175->Text = L"okres:";
			// 
			// textBox103
			// 
			this->textBox103->Location = System::Drawing::Point(67, 6);
			this->textBox103->Name = L"textBox103";
			this->textBox103->Size = System::Drawing::Size(123, 20);
			this->textBox103->TabIndex = 11;
			// 
			// label176
			// 
			this->label176->AutoSize = true;
			this->label176->Location = System::Drawing::Point(6, 9);
			this->label176->Name = L"label176";
			this->label176->Size = System::Drawing::Size(55, 13);
			this->label176->TabIndex = 10;
			this->label176->Text = L"amplituda:";
			// 
			// tabPage63
			// 
			this->tabPage63->Controls->Add(this->textBox104);
			this->tabPage63->Controls->Add(this->label177);
			this->tabPage63->Controls->Add(this->textBox105);
			this->tabPage63->Controls->Add(this->label178);
			this->tabPage63->Location = System::Drawing::Point(4, 22);
			this->tabPage63->Name = L"tabPage63";
			this->tabPage63->Size = System::Drawing::Size(309, 86);
			this->tabPage63->TabIndex = 4;
			this->tabPage63->Text = L"Szum";
			this->tabPage63->UseVisualStyleBackColor = true;
			// 
			// textBox104
			// 
			this->textBox104->Location = System::Drawing::Point(82, 32);
			this->textBox104->Name = L"textBox104";
			this->textBox104->Size = System::Drawing::Size(108, 20);
			this->textBox104->TabIndex = 13;
			// 
			// label177
			// 
			this->label177->AutoSize = true;
			this->label177->Location = System::Drawing::Point(6, 35);
			this->label177->Name = L"label177";
			this->label177->Size = System::Drawing::Size(73, 13);
			this->label177->TabIndex = 12;
			this->label177->Text = L"liczba próbek:";
			// 
			// textBox105
			// 
			this->textBox105->Location = System::Drawing::Point(82, 7);
			this->textBox105->Name = L"textBox105";
			this->textBox105->Size = System::Drawing::Size(108, 20);
			this->textBox105->TabIndex = 11;
			// 
			// label178
			// 
			this->label178->AutoSize = true;
			this->label178->Location = System::Drawing::Point(6, 10);
			this->label178->Name = L"label178";
			this->label178->Size = System::Drawing::Size(55, 13);
			this->label178->TabIndex = 10;
			this->label178->Text = L"wariancja:";
			// 
			// textBox106
			// 
			this->textBox106->Location = System::Drawing::Point(98, 431);
			this->textBox106->Name = L"textBox106";
			this->textBox106->Size = System::Drawing::Size(237, 20);
			this->textBox106->TabIndex = 14;
			// 
			// label179
			// 
			this->label179->AutoSize = true;
			this->label179->Location = System::Drawing::Point(5, 434);
			this->label179->Name = L"label179";
			this->label179->Size = System::Drawing::Size(27, 13);
			this->label179->TabIndex = 13;
			this->label179->Text = L"Plik:";
			// 
			// groupBox21
			// 
			this->groupBox21->Controls->Add(this->button81);
			this->groupBox21->Controls->Add(this->label180);
			this->groupBox21->Controls->Add(this->textBox107);
			this->groupBox21->ForeColor = System::Drawing::Color::Black;
			this->groupBox21->Location = System::Drawing::Point(6, 6);
			this->groupBox21->Name = L"groupBox21";
			this->groupBox21->Size = System::Drawing::Size(333, 79);
			this->groupBox21->TabIndex = 15;
			this->groupBox21->TabStop = false;
			this->groupBox21->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// button81
			// 
			this->button81->Location = System::Drawing::Point(252, 45);
			this->button81->Name = L"button81";
			this->button81->Size = System::Drawing::Size(75, 23);
			this->button81->TabIndex = 2;
			this->button81->Text = L"Wczytaj";
			this->button81->UseVisualStyleBackColor = true;
			// 
			// label180
			// 
			this->label180->AutoSize = true;
			this->label180->Location = System::Drawing::Point(6, 22);
			this->label180->Name = L"label180";
			this->label180->Size = System::Drawing::Size(93, 13);
			this->label180->TabIndex = 1;
			this->label180->Text = L"Nazwa regulatora:";
			// 
			// textBox107
			// 
			this->textBox107->Location = System::Drawing::Point(105, 19);
			this->textBox107->Name = L"textBox107";
			this->textBox107->Size = System::Drawing::Size(222, 20);
			this->textBox107->TabIndex = 0;
			// 
			// textBox108
			// 
			this->textBox108->Location = System::Drawing::Point(98, 405);
			this->textBox108->Name = L"textBox108";
			this->textBox108->Size = System::Drawing::Size(237, 20);
			this->textBox108->TabIndex = 12;
			// 
			// label181
			// 
			this->label181->AutoSize = true;
			this->label181->Location = System::Drawing::Point(4, 408);
			this->label181->Name = L"label181";
			this->label181->Size = System::Drawing::Size(93, 13);
			this->label181->TabIndex = 11;
			this->label181->Text = L"Nazwa regulatora:";
			// 
			// button82
			// 
			this->button82->Location = System::Drawing::Point(245, 457);
			this->button82->Name = L"button82";
			this->button82->Size = System::Drawing::Size(90, 23);
			this->button82->TabIndex = 10;
			this->button82->Text = L"Zapisz";
			this->button82->UseVisualStyleBackColor = true;
			// 
			// tabPage64
			// 
			this->tabPage64->Controls->Add(this->tabControl16);
			this->tabPage64->Controls->Add(this->textBox128);
			this->tabPage64->Controls->Add(this->label201);
			this->tabPage64->Controls->Add(this->groupBox23);
			this->tabPage64->Controls->Add(this->textBox130);
			this->tabPage64->Controls->Add(this->label203);
			this->tabPage64->Controls->Add(this->button95);
			this->tabPage64->Location = System::Drawing::Point(4, 22);
			this->tabPage64->Name = L"tabPage64";
			this->tabPage64->Size = System::Drawing::Size(345, 494);
			this->tabPage64->TabIndex = 2;
			this->tabPage64->Text = L"Regulator PID";
			this->tabPage64->UseVisualStyleBackColor = true;
			// 
			// tabControl16
			// 
			this->tabControl16->Controls->Add(this->tabPage65);
			this->tabControl16->Controls->Add(this->tabPage66);
			this->tabControl16->Location = System::Drawing::Point(6, 91);
			this->tabControl16->Name = L"tabControl16";
			this->tabControl16->SelectedIndex = 0;
			this->tabControl16->Size = System::Drawing::Size(333, 308);
			this->tabControl16->TabIndex = 20;
			// 
			// tabPage65
			// 
			this->tabPage65->Controls->Add(this->button83);
			this->tabPage65->Controls->Add(this->textBox109);
			this->tabPage65->Controls->Add(this->label182);
			this->tabPage65->Controls->Add(this->button84);
			this->tabPage65->Controls->Add(this->textBox110);
			this->tabPage65->Controls->Add(this->label183);
			this->tabPage65->Controls->Add(this->button85);
			this->tabPage65->Controls->Add(this->button86);
			this->tabPage65->Controls->Add(this->textBox111);
			this->tabPage65->Controls->Add(this->label184);
			this->tabPage65->Controls->Add(this->button87);
			this->tabPage65->Controls->Add(this->textBox112);
			this->tabPage65->Controls->Add(this->label185);
			this->tabPage65->Controls->Add(this->button88);
			this->tabPage65->Controls->Add(this->textBox113);
			this->tabPage65->Controls->Add(this->label186);
			this->tabPage65->Controls->Add(this->button89);
			this->tabPage65->Controls->Add(this->textBox114);
			this->tabPage65->Controls->Add(this->label187);
			this->tabPage65->Controls->Add(this->button90);
			this->tabPage65->Controls->Add(this->textBox115);
			this->tabPage65->Controls->Add(this->label188);
			this->tabPage65->Controls->Add(this->button91);
			this->tabPage65->Controls->Add(this->textBox116);
			this->tabPage65->Controls->Add(this->label189);
			this->tabPage65->Controls->Add(this->button92);
			this->tabPage65->Controls->Add(this->textBox117);
			this->tabPage65->Controls->Add(this->label190);
			this->tabPage65->Location = System::Drawing::Point(4, 22);
			this->tabPage65->Name = L"tabPage65";
			this->tabPage65->Padding = System::Windows::Forms::Padding(3);
			this->tabPage65->Size = System::Drawing::Size(325, 282);
			this->tabPage65->TabIndex = 0;
			this->tabPage65->Text = L"Zmiana parametrów";
			this->tabPage65->UseVisualStyleBackColor = true;
			// 
			// button83
			// 
			this->button83->Location = System::Drawing::Point(273, 212);
			this->button83->Name = L"button83";
			this->button83->Size = System::Drawing::Size(46, 23);
			this->button83->TabIndex = 43;
			this->button83->Text = L"Zmieñ";
			this->button83->UseVisualStyleBackColor = true;
			// 
			// textBox109
			// 
			this->textBox109->Location = System::Drawing::Point(101, 214);
			this->textBox109->Name = L"textBox109";
			this->textBox109->Size = System::Drawing::Size(166, 20);
			this->textBox109->TabIndex = 42;
			// 
			// label182
			// 
			this->label182->AutoSize = true;
			this->label182->Location = System::Drawing::Point(3, 217);
			this->label182->Name = L"label182";
			this->label182->Size = System::Drawing::Size(49, 13);
			this->label182->TabIndex = 41;
			this->label182->Text = L"Wy_Min:";
			// 
			// button84
			// 
			this->button84->Location = System::Drawing::Point(273, 186);
			this->button84->Name = L"button84";
			this->button84->Size = System::Drawing::Size(46, 23);
			this->button84->TabIndex = 40;
			this->button84->Text = L"Zmieñ";
			this->button84->UseVisualStyleBackColor = true;
			// 
			// textBox110
			// 
			this->textBox110->Location = System::Drawing::Point(101, 188);
			this->textBox110->Name = L"textBox110";
			this->textBox110->Size = System::Drawing::Size(166, 20);
			this->textBox110->TabIndex = 39;
			// 
			// label183
			// 
			this->label183->AutoSize = true;
			this->label183->Location = System::Drawing::Point(3, 191);
			this->label183->Name = L"label183";
			this->label183->Size = System::Drawing::Size(52, 13);
			this->label183->TabIndex = 38;
			this->label183->Text = L"Wy_Max:";
			// 
			// button85
			// 
			this->button85->Location = System::Drawing::Point(196, 253);
			this->button85->Name = L"button85";
			this->button85->Size = System::Drawing::Size(123, 23);
			this->button85->TabIndex = 37;
			this->button85->Text = L"Zmieñ wszystko";
			this->button85->UseVisualStyleBackColor = true;
			// 
			// button86
			// 
			this->button86->Location = System::Drawing::Point(273, 160);
			this->button86->Name = L"button86";
			this->button86->Size = System::Drawing::Size(46, 23);
			this->button86->TabIndex = 25;
			this->button86->Text = L"Zmieñ";
			this->button86->UseVisualStyleBackColor = true;
			// 
			// textBox111
			// 
			this->textBox111->Location = System::Drawing::Point(101, 162);
			this->textBox111->Name = L"textBox111";
			this->textBox111->Size = System::Drawing::Size(166, 20);
			this->textBox111->TabIndex = 24;
			// 
			// label184
			// 
			this->label184->AutoSize = true;
			this->label184->Location = System::Drawing::Point(3, 165);
			this->label184->Name = L"label184";
			this->label184->Size = System::Drawing::Size(16, 13);
			this->label184->TabIndex = 23;
			this->label184->Text = L"b:";
			// 
			// button87
			// 
			this->button87->Location = System::Drawing::Point(273, 134);
			this->button87->Name = L"button87";
			this->button87->Size = System::Drawing::Size(46, 23);
			this->button87->TabIndex = 22;
			this->button87->Text = L"Zmieñ";
			this->button87->UseVisualStyleBackColor = true;
			// 
			// textBox112
			// 
			this->textBox112->Location = System::Drawing::Point(101, 136);
			this->textBox112->Name = L"textBox112";
			this->textBox112->Size = System::Drawing::Size(166, 20);
			this->textBox112->TabIndex = 21;
			// 
			// label185
			// 
			this->label185->AutoSize = true;
			this->label185->Location = System::Drawing::Point(3, 139);
			this->label185->Name = L"label185";
			this->label185->Size = System::Drawing::Size(18, 13);
			this->label185->TabIndex = 20;
			this->label185->Text = L"N:";
			// 
			// button88
			// 
			this->button88->Location = System::Drawing::Point(273, 108);
			this->button88->Name = L"button88";
			this->button88->Size = System::Drawing::Size(46, 23);
			this->button88->TabIndex = 19;
			this->button88->Text = L"Zmieñ";
			this->button88->UseVisualStyleBackColor = true;
			// 
			// textBox113
			// 
			this->textBox113->Location = System::Drawing::Point(101, 110);
			this->textBox113->Name = L"textBox113";
			this->textBox113->Size = System::Drawing::Size(166, 20);
			this->textBox113->TabIndex = 18;
			// 
			// label186
			// 
			this->label186->AutoSize = true;
			this->label186->Location = System::Drawing::Point(3, 113);
			this->label186->Name = L"label186";
			this->label186->Size = System::Drawing::Size(87, 13);
			this->label186->TabIndex = 17;
			this->label186->Text = L"WP ca³kowania:";
			// 
			// button89
			// 
			this->button89->Location = System::Drawing::Point(273, 82);
			this->button89->Name = L"button89";
			this->button89->Size = System::Drawing::Size(46, 23);
			this->button89->TabIndex = 16;
			this->button89->Text = L"Zmieñ";
			this->button89->UseVisualStyleBackColor = true;
			// 
			// textBox114
			// 
			this->textBox114->Location = System::Drawing::Point(101, 84);
			this->textBox114->Name = L"textBox114";
			this->textBox114->Size = System::Drawing::Size(166, 20);
			this->textBox114->TabIndex = 15;
			// 
			// label187
			// 
			this->label187->AutoSize = true;
			this->label187->Location = System::Drawing::Point(3, 87);
			this->label187->Name = L"label187";
			this->label187->Size = System::Drawing::Size(23, 13);
			this->label187->TabIndex = 14;
			this->label187->Text = L"Tp:";
			// 
			// button90
			// 
			this->button90->Location = System::Drawing::Point(273, 56);
			this->button90->Name = L"button90";
			this->button90->Size = System::Drawing::Size(46, 23);
			this->button90->TabIndex = 13;
			this->button90->Text = L"Zmieñ";
			this->button90->UseVisualStyleBackColor = true;
			// 
			// textBox115
			// 
			this->textBox115->Location = System::Drawing::Point(101, 58);
			this->textBox115->Name = L"textBox115";
			this->textBox115->Size = System::Drawing::Size(166, 20);
			this->textBox115->TabIndex = 12;
			// 
			// label188
			// 
			this->label188->AutoSize = true;
			this->label188->Location = System::Drawing::Point(3, 61);
			this->label188->Name = L"label188";
			this->label188->Size = System::Drawing::Size(23, 13);
			this->label188->TabIndex = 11;
			this->label188->Text = L"Td:";
			// 
			// button91
			// 
			this->button91->Location = System::Drawing::Point(273, 30);
			this->button91->Name = L"button91";
			this->button91->Size = System::Drawing::Size(46, 23);
			this->button91->TabIndex = 10;
			this->button91->Text = L"Zmieñ";
			this->button91->UseVisualStyleBackColor = true;
			// 
			// textBox116
			// 
			this->textBox116->Location = System::Drawing::Point(101, 32);
			this->textBox116->Name = L"textBox116";
			this->textBox116->Size = System::Drawing::Size(166, 20);
			this->textBox116->TabIndex = 9;
			// 
			// label189
			// 
			this->label189->AutoSize = true;
			this->label189->Location = System::Drawing::Point(3, 35);
			this->label189->Name = L"label189";
			this->label189->Size = System::Drawing::Size(19, 13);
			this->label189->TabIndex = 8;
			this->label189->Text = L"Ti:";
			// 
			// button92
			// 
			this->button92->Location = System::Drawing::Point(273, 4);
			this->button92->Name = L"button92";
			this->button92->Size = System::Drawing::Size(46, 23);
			this->button92->TabIndex = 7;
			this->button92->Text = L"Zmieñ";
			this->button92->UseVisualStyleBackColor = true;
			// 
			// textBox117
			// 
			this->textBox117->Location = System::Drawing::Point(101, 6);
			this->textBox117->Name = L"textBox117";
			this->textBox117->Size = System::Drawing::Size(166, 20);
			this->textBox117->TabIndex = 6;
			// 
			// label190
			// 
			this->label190->AutoSize = true;
			this->label190->Location = System::Drawing::Point(3, 9);
			this->label190->Name = L"label190";
			this->label190->Size = System::Drawing::Size(16, 13);
			this->label190->TabIndex = 5;
			this->label190->Text = L"k:";
			// 
			// tabPage66
			// 
			this->tabPage66->Controls->Add(this->button93);
			this->tabPage66->Controls->Add(this->groupBox22);
			this->tabPage66->Controls->Add(this->tabControl17);
			this->tabPage66->Location = System::Drawing::Point(4, 22);
			this->tabPage66->Name = L"tabPage66";
			this->tabPage66->Padding = System::Windows::Forms::Padding(3);
			this->tabPage66->Size = System::Drawing::Size(325, 282);
			this->tabPage66->TabIndex = 1;
			this->tabPage66->Text = L"Zmiana wymuszenia";
			this->tabPage66->UseVisualStyleBackColor = true;
			// 
			// button93
			// 
			this->button93->Location = System::Drawing::Point(244, 253);
			this->button93->Name = L"button93";
			this->button93->Size = System::Drawing::Size(75, 23);
			this->button93->TabIndex = 9;
			this->button93->Text = L"Zmieñ";
			this->button93->UseVisualStyleBackColor = true;
			// 
			// groupBox22
			// 
			this->groupBox22->Controls->Add(this->checkBox26);
			this->groupBox22->Controls->Add(this->checkBox27);
			this->groupBox22->Controls->Add(this->checkBox28);
			this->groupBox22->Controls->Add(this->checkBox29);
			this->groupBox22->Controls->Add(this->checkBox30);
			this->groupBox22->Controls->Add(this->checkBox31);
			this->groupBox22->ForeColor = System::Drawing::Color::Black;
			this->groupBox22->Location = System::Drawing::Point(3, 8);
			this->groupBox22->Name = L"groupBox22";
			this->groupBox22->Size = System::Drawing::Size(316, 89);
			this->groupBox22->TabIndex = 7;
			this->groupBox22->TabStop = false;
			this->groupBox22->Text = L"Sk³adowe wymuszenia";
			// 
			// checkBox26
			// 
			this->checkBox26->AutoSize = true;
			this->checkBox26->Location = System::Drawing::Point(6, 19);
			this->checkBox26->Name = L"checkBox26";
			this->checkBox26->Size = System::Drawing::Size(51, 17);
			this->checkBox26->TabIndex = 1;
			this->checkBox26->Text = L"Skok";
			this->checkBox26->UseVisualStyleBackColor = true;
			// 
			// checkBox27
			// 
			this->checkBox27->AutoSize = true;
			this->checkBox27->Location = System::Drawing::Point(128, 65);
			this->checkBox27->Name = L"checkBox27";
			this->checkBox27->Size = System::Drawing::Size(52, 17);
			this->checkBox27->TabIndex = 6;
			this->checkBox27->Text = L"Szum";
			this->checkBox27->UseVisualStyleBackColor = true;
			// 
			// checkBox28
			// 
			this->checkBox28->AutoSize = true;
			this->checkBox28->Location = System::Drawing::Point(6, 42);
			this->checkBox28->Name = L"checkBox28";
			this->checkBox28->Size = System::Drawing::Size(108, 17);
			this->checkBox28->TabIndex = 2;
			this->checkBox28->Text = L"Impuls Kronekera";
			this->checkBox28->UseVisualStyleBackColor = true;
			// 
			// checkBox29
			// 
			this->checkBox29->AutoSize = true;
			this->checkBox29->Location = System::Drawing::Point(128, 42);
			this->checkBox29->Name = L"checkBox29";
			this->checkBox29->Size = System::Drawing::Size(59, 17);
			this->checkBox29->TabIndex = 5;
			this->checkBox29->Text = L"Trójk¹t";
			this->checkBox29->UseVisualStyleBackColor = true;
			// 
			// checkBox30
			// 
			this->checkBox30->AutoSize = true;
			this->checkBox30->Location = System::Drawing::Point(6, 65);
			this->checkBox30->Name = L"checkBox30";
			this->checkBox30->Size = System::Drawing::Size(71, 17);
			this->checkBox30->TabIndex = 3;
			this->checkBox30->Text = L"Prostok¹t";
			this->checkBox30->UseVisualStyleBackColor = true;
			// 
			// checkBox31
			// 
			this->checkBox31->AutoSize = true;
			this->checkBox31->Location = System::Drawing::Point(128, 19);
			this->checkBox31->Name = L"checkBox31";
			this->checkBox31->Size = System::Drawing::Size(52, 17);
			this->checkBox31->TabIndex = 4;
			this->checkBox31->Text = L"Sinus";
			this->checkBox31->UseVisualStyleBackColor = true;
			// 
			// tabControl17
			// 
			this->tabControl17->Controls->Add(this->tabPage67);
			this->tabControl17->Controls->Add(this->tabPage68);
			this->tabControl17->Controls->Add(this->tabPage69);
			this->tabControl17->Controls->Add(this->tabPage70);
			this->tabControl17->Controls->Add(this->tabPage71);
			this->tabControl17->Location = System::Drawing::Point(2, 103);
			this->tabControl17->Multiline = true;
			this->tabControl17->Name = L"tabControl17";
			this->tabControl17->SelectedIndex = 0;
			this->tabControl17->Size = System::Drawing::Size(316, 112);
			this->tabControl17->TabIndex = 8;
			// 
			// tabPage67
			// 
			this->tabPage67->Controls->Add(this->textBox118);
			this->tabPage67->Controls->Add(this->label191);
			this->tabPage67->Location = System::Drawing::Point(4, 22);
			this->tabPage67->Name = L"tabPage67";
			this->tabPage67->Padding = System::Windows::Forms::Padding(3);
			this->tabPage67->Size = System::Drawing::Size(308, 86);
			this->tabPage67->TabIndex = 0;
			this->tabPage67->Text = L"Skok";
			this->tabPage67->UseVisualStyleBackColor = true;
			// 
			// textBox118
			// 
			this->textBox118->Location = System::Drawing::Point(91, 6);
			this->textBox118->Name = L"textBox118";
			this->textBox118->Size = System::Drawing::Size(99, 20);
			this->textBox118->TabIndex = 1;
			// 
			// label191
			// 
			this->label191->AutoSize = true;
			this->label191->Location = System::Drawing::Point(6, 9);
			this->label191->Name = L"label191";
			this->label191->Size = System::Drawing::Size(79, 13);
			this->label191->TabIndex = 0;
			this->label191->Text = L"wartoœæ skoku:";
			// 
			// tabPage68
			// 
			this->tabPage68->Controls->Add(this->textBox119);
			this->tabPage68->Controls->Add(this->label192);
			this->tabPage68->Controls->Add(this->textBox120);
			this->tabPage68->Controls->Add(this->label193);
			this->tabPage68->Controls->Add(this->textBox121);
			this->tabPage68->Controls->Add(this->label194);
			this->tabPage68->Location = System::Drawing::Point(4, 22);
			this->tabPage68->Name = L"tabPage68";
			this->tabPage68->Padding = System::Windows::Forms::Padding(3);
			this->tabPage68->Size = System::Drawing::Size(308, 86);
			this->tabPage68->TabIndex = 1;
			this->tabPage68->Text = L"Prostok¹t";
			this->tabPage68->UseVisualStyleBackColor = true;
			// 
			// textBox119
			// 
			this->textBox119->Location = System::Drawing::Point(79, 58);
			this->textBox119->Name = L"textBox119";
			this->textBox119->Size = System::Drawing::Size(111, 20);
			this->textBox119->TabIndex = 7;
			// 
			// label192
			// 
			this->label192->AutoSize = true;
			this->label192->Location = System::Drawing::Point(6, 61);
			this->label192->Name = L"label192";
			this->label192->Size = System::Drawing::Size(67, 13);
			this->label192->TabIndex = 6;
			this->label192->Text = L"wype³nienie:";
			// 
			// textBox120
			// 
			this->textBox120->Location = System::Drawing::Point(79, 32);
			this->textBox120->Name = L"textBox120";
			this->textBox120->Size = System::Drawing::Size(111, 20);
			this->textBox120->TabIndex = 5;
			// 
			// label193
			// 
			this->label193->AutoSize = true;
			this->label193->Location = System::Drawing::Point(6, 35);
			this->label193->Name = L"label193";
			this->label193->Size = System::Drawing::Size(36, 13);
			this->label193->TabIndex = 4;
			this->label193->Text = L"okres:";
			// 
			// textBox121
			// 
			this->textBox121->Location = System::Drawing::Point(79, 6);
			this->textBox121->Name = L"textBox121";
			this->textBox121->Size = System::Drawing::Size(111, 20);
			this->textBox121->TabIndex = 3;
			// 
			// label194
			// 
			this->label194->AutoSize = true;
			this->label194->Location = System::Drawing::Point(6, 9);
			this->label194->Name = L"label194";
			this->label194->Size = System::Drawing::Size(55, 13);
			this->label194->TabIndex = 2;
			this->label194->Text = L"amplituda:";
			// 
			// tabPage69
			// 
			this->tabPage69->Controls->Add(this->textBox122);
			this->tabPage69->Controls->Add(this->label195);
			this->tabPage69->Controls->Add(this->textBox123);
			this->tabPage69->Controls->Add(this->label196);
			this->tabPage69->Location = System::Drawing::Point(4, 22);
			this->tabPage69->Name = L"tabPage69";
			this->tabPage69->Size = System::Drawing::Size(308, 86);
			this->tabPage69->TabIndex = 2;
			this->tabPage69->Text = L"Sinus";
			this->tabPage69->UseVisualStyleBackColor = true;
			// 
			// textBox122
			// 
			this->textBox122->Location = System::Drawing::Point(67, 32);
			this->textBox122->Name = L"textBox122";
			this->textBox122->Size = System::Drawing::Size(123, 20);
			this->textBox122->TabIndex = 9;
			// 
			// label195
			// 
			this->label195->AutoSize = true;
			this->label195->Location = System::Drawing::Point(6, 35);
			this->label195->Name = L"label195";
			this->label195->Size = System::Drawing::Size(36, 13);
			this->label195->TabIndex = 8;
			this->label195->Text = L"okres:";
			// 
			// textBox123
			// 
			this->textBox123->Location = System::Drawing::Point(67, 6);
			this->textBox123->Name = L"textBox123";
			this->textBox123->Size = System::Drawing::Size(123, 20);
			this->textBox123->TabIndex = 7;
			// 
			// label196
			// 
			this->label196->AutoSize = true;
			this->label196->Location = System::Drawing::Point(6, 9);
			this->label196->Name = L"label196";
			this->label196->Size = System::Drawing::Size(55, 13);
			this->label196->TabIndex = 6;
			this->label196->Text = L"amplituda:";
			// 
			// tabPage70
			// 
			this->tabPage70->Controls->Add(this->textBox124);
			this->tabPage70->Controls->Add(this->label197);
			this->tabPage70->Controls->Add(this->textBox125);
			this->tabPage70->Controls->Add(this->label198);
			this->tabPage70->Location = System::Drawing::Point(4, 22);
			this->tabPage70->Name = L"tabPage70";
			this->tabPage70->Size = System::Drawing::Size(308, 86);
			this->tabPage70->TabIndex = 3;
			this->tabPage70->Text = L"Trójk¹t";
			this->tabPage70->UseVisualStyleBackColor = true;
			// 
			// textBox124
			// 
			this->textBox124->Location = System::Drawing::Point(67, 32);
			this->textBox124->Name = L"textBox124";
			this->textBox124->Size = System::Drawing::Size(123, 20);
			this->textBox124->TabIndex = 13;
			// 
			// label197
			// 
			this->label197->AutoSize = true;
			this->label197->Location = System::Drawing::Point(6, 35);
			this->label197->Name = L"label197";
			this->label197->Size = System::Drawing::Size(36, 13);
			this->label197->TabIndex = 12;
			this->label197->Text = L"okres:";
			// 
			// textBox125
			// 
			this->textBox125->Location = System::Drawing::Point(67, 7);
			this->textBox125->Name = L"textBox125";
			this->textBox125->Size = System::Drawing::Size(123, 20);
			this->textBox125->TabIndex = 11;
			// 
			// label198
			// 
			this->label198->AutoSize = true;
			this->label198->Location = System::Drawing::Point(6, 9);
			this->label198->Name = L"label198";
			this->label198->Size = System::Drawing::Size(55, 13);
			this->label198->TabIndex = 10;
			this->label198->Text = L"amplituda:";
			// 
			// tabPage71
			// 
			this->tabPage71->Controls->Add(this->textBox126);
			this->tabPage71->Controls->Add(this->label199);
			this->tabPage71->Controls->Add(this->textBox127);
			this->tabPage71->Controls->Add(this->label200);
			this->tabPage71->Location = System::Drawing::Point(4, 22);
			this->tabPage71->Name = L"tabPage71";
			this->tabPage71->Size = System::Drawing::Size(308, 86);
			this->tabPage71->TabIndex = 4;
			this->tabPage71->Text = L"Szum";
			this->tabPage71->UseVisualStyleBackColor = true;
			// 
			// textBox126
			// 
			this->textBox126->Location = System::Drawing::Point(82, 32);
			this->textBox126->Name = L"textBox126";
			this->textBox126->Size = System::Drawing::Size(108, 20);
			this->textBox126->TabIndex = 13;
			// 
			// label199
			// 
			this->label199->AutoSize = true;
			this->label199->Location = System::Drawing::Point(6, 35);
			this->label199->Name = L"label199";
			this->label199->Size = System::Drawing::Size(73, 13);
			this->label199->TabIndex = 12;
			this->label199->Text = L"liczba próbek:";
			// 
			// textBox127
			// 
			this->textBox127->Location = System::Drawing::Point(82, 7);
			this->textBox127->Name = L"textBox127";
			this->textBox127->Size = System::Drawing::Size(108, 20);
			this->textBox127->TabIndex = 11;
			// 
			// label200
			// 
			this->label200->AutoSize = true;
			this->label200->Location = System::Drawing::Point(6, 10);
			this->label200->Name = L"label200";
			this->label200->Size = System::Drawing::Size(55, 13);
			this->label200->TabIndex = 10;
			this->label200->Text = L"wariancja:";
			// 
			// textBox128
			// 
			this->textBox128->Location = System::Drawing::Point(98, 431);
			this->textBox128->Name = L"textBox128";
			this->textBox128->Size = System::Drawing::Size(237, 20);
			this->textBox128->TabIndex = 19;
			// 
			// label201
			// 
			this->label201->AutoSize = true;
			this->label201->Location = System::Drawing::Point(5, 434);
			this->label201->Name = L"label201";
			this->label201->Size = System::Drawing::Size(27, 13);
			this->label201->TabIndex = 18;
			this->label201->Text = L"Plik:";
			// 
			// groupBox23
			// 
			this->groupBox23->Controls->Add(this->button94);
			this->groupBox23->Controls->Add(this->label202);
			this->groupBox23->Controls->Add(this->textBox129);
			this->groupBox23->ForeColor = System::Drawing::Color::Black;
			this->groupBox23->Location = System::Drawing::Point(6, 6);
			this->groupBox23->Name = L"groupBox23";
			this->groupBox23->Size = System::Drawing::Size(333, 79);
			this->groupBox23->TabIndex = 16;
			this->groupBox23->TabStop = false;
			this->groupBox23->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// button94
			// 
			this->button94->Location = System::Drawing::Point(252, 45);
			this->button94->Name = L"button94";
			this->button94->Size = System::Drawing::Size(75, 23);
			this->button94->TabIndex = 2;
			this->button94->Text = L"Wczytaj";
			this->button94->UseVisualStyleBackColor = true;
			// 
			// label202
			// 
			this->label202->AutoSize = true;
			this->label202->Location = System::Drawing::Point(6, 22);
			this->label202->Name = L"label202";
			this->label202->Size = System::Drawing::Size(93, 13);
			this->label202->TabIndex = 1;
			this->label202->Text = L"Nazwa regulatora:";
			// 
			// textBox129
			// 
			this->textBox129->Location = System::Drawing::Point(105, 19);
			this->textBox129->Name = L"textBox129";
			this->textBox129->Size = System::Drawing::Size(222, 20);
			this->textBox129->TabIndex = 0;
			// 
			// textBox130
			// 
			this->textBox130->Location = System::Drawing::Point(98, 405);
			this->textBox130->Name = L"textBox130";
			this->textBox130->Size = System::Drawing::Size(237, 20);
			this->textBox130->TabIndex = 17;
			// 
			// label203
			// 
			this->label203->AutoSize = true;
			this->label203->Location = System::Drawing::Point(4, 408);
			this->label203->Name = L"label203";
			this->label203->Size = System::Drawing::Size(93, 13);
			this->label203->TabIndex = 16;
			this->label203->Text = L"Nazwa regulatora:";
			// 
			// button95
			// 
			this->button95->Location = System::Drawing::Point(249, 457);
			this->button95->Name = L"button95";
			this->button95->Size = System::Drawing::Size(90, 23);
			this->button95->TabIndex = 15;
			this->button95->Text = L"Zapisz";
			this->button95->UseVisualStyleBackColor = true;
			// 
			// tabPage72
			// 
			this->tabPage72->Controls->Add(this->textBox131);
			this->tabPage72->Controls->Add(this->label204);
			this->tabPage72->Controls->Add(this->textBox132);
			this->tabPage72->Controls->Add(this->label205);
			this->tabPage72->Controls->Add(this->button96);
			this->tabPage72->Controls->Add(this->tabControl18);
			this->tabPage72->Controls->Add(this->groupBox25);
			this->tabPage72->Location = System::Drawing::Point(4, 22);
			this->tabPage72->Name = L"tabPage72";
			this->tabPage72->Size = System::Drawing::Size(345, 494);
			this->tabPage72->TabIndex = 3;
			this->tabPage72->Text = L"Regulator GPC";
			this->tabPage72->UseVisualStyleBackColor = true;
			// 
			// textBox131
			// 
			this->textBox131->Location = System::Drawing::Point(98, 431);
			this->textBox131->Name = L"textBox131";
			this->textBox131->Size = System::Drawing::Size(237, 20);
			this->textBox131->TabIndex = 26;
			// 
			// label204
			// 
			this->label204->AutoSize = true;
			this->label204->Location = System::Drawing::Point(5, 434);
			this->label204->Name = L"label204";
			this->label204->Size = System::Drawing::Size(27, 13);
			this->label204->TabIndex = 25;
			this->label204->Text = L"Plik:";
			// 
			// textBox132
			// 
			this->textBox132->Location = System::Drawing::Point(98, 405);
			this->textBox132->Name = L"textBox132";
			this->textBox132->Size = System::Drawing::Size(237, 20);
			this->textBox132->TabIndex = 24;
			// 
			// label205
			// 
			this->label205->AutoSize = true;
			this->label205->Location = System::Drawing::Point(4, 408);
			this->label205->Name = L"label205";
			this->label205->Size = System::Drawing::Size(93, 13);
			this->label205->TabIndex = 23;
			this->label205->Text = L"Nazwa regulatora:";
			// 
			// button96
			// 
			this->button96->Location = System::Drawing::Point(249, 457);
			this->button96->Name = L"button96";
			this->button96->Size = System::Drawing::Size(90, 23);
			this->button96->TabIndex = 22;
			this->button96->Text = L"Zapisz";
			this->button96->UseVisualStyleBackColor = true;
			// 
			// tabControl18
			// 
			this->tabControl18->Controls->Add(this->tabPage73);
			this->tabControl18->Controls->Add(this->tabPage74);
			this->tabControl18->Controls->Add(this->tabPage80);
			this->tabControl18->Location = System::Drawing::Point(6, 91);
			this->tabControl18->Name = L"tabControl18";
			this->tabControl18->SelectedIndex = 0;
			this->tabControl18->Size = System::Drawing::Size(333, 308);
			this->tabControl18->TabIndex = 21;
			// 
			// tabPage73
			// 
			this->tabPage73->Controls->Add(this->button97);
			this->tabPage73->Controls->Add(this->textBox133);
			this->tabPage73->Controls->Add(this->label206);
			this->tabPage73->Controls->Add(this->button98);
			this->tabPage73->Controls->Add(this->textBox134);
			this->tabPage73->Controls->Add(this->label207);
			this->tabPage73->Controls->Add(this->button99);
			this->tabPage73->Controls->Add(this->textBox135);
			this->tabPage73->Controls->Add(this->label208);
			this->tabPage73->Controls->Add(this->button100);
			this->tabPage73->Controls->Add(this->textBox136);
			this->tabPage73->Controls->Add(this->label209);
			this->tabPage73->Controls->Add(this->button101);
			this->tabPage73->Controls->Add(this->button102);
			this->tabPage73->Controls->Add(this->textBox137);
			this->tabPage73->Controls->Add(this->label210);
			this->tabPage73->Controls->Add(this->button103);
			this->tabPage73->Controls->Add(this->textBox138);
			this->tabPage73->Controls->Add(this->label211);
			this->tabPage73->Controls->Add(this->button104);
			this->tabPage73->Controls->Add(this->textBox139);
			this->tabPage73->Controls->Add(this->label212);
			this->tabPage73->Controls->Add(this->button105);
			this->tabPage73->Controls->Add(this->textBox140);
			this->tabPage73->Controls->Add(this->label213);
			this->tabPage73->Location = System::Drawing::Point(4, 22);
			this->tabPage73->Name = L"tabPage73";
			this->tabPage73->Padding = System::Windows::Forms::Padding(3);
			this->tabPage73->Size = System::Drawing::Size(325, 282);
			this->tabPage73->TabIndex = 0;
			this->tabPage73->Text = L"Zmiana parametrów";
			this->tabPage73->UseVisualStyleBackColor = true;
			// 
			// button97
			// 
			this->button97->Location = System::Drawing::Point(274, 133);
			this->button97->Name = L"button97";
			this->button97->Size = System::Drawing::Size(46, 23);
			this->button97->TabIndex = 49;
			this->button97->Text = L"Zmieñ";
			this->button97->UseVisualStyleBackColor = true;
			// 
			// textBox133
			// 
			this->textBox133->Location = System::Drawing::Point(102, 135);
			this->textBox133->Name = L"textBox133";
			this->textBox133->Size = System::Drawing::Size(166, 20);
			this->textBox133->TabIndex = 48;
			// 
			// label206
			// 
			this->label206->AutoSize = true;
			this->label206->Location = System::Drawing::Point(4, 138);
			this->label206->Name = L"label206";
			this->label206->Size = System::Drawing::Size(20, 13);
			this->label206->TabIndex = 47;
			this->label206->Text = L"dB";
			// 
			// button98
			// 
			this->button98->Location = System::Drawing::Point(274, 107);
			this->button98->Name = L"button98";
			this->button98->Size = System::Drawing::Size(46, 23);
			this->button98->TabIndex = 46;
			this->button98->Text = L"Zmieñ";
			this->button98->UseVisualStyleBackColor = true;
			// 
			// textBox134
			// 
			this->textBox134->Location = System::Drawing::Point(102, 109);
			this->textBox134->Name = L"textBox134";
			this->textBox134->Size = System::Drawing::Size(166, 20);
			this->textBox134->TabIndex = 45;
			// 
			// label207
			// 
			this->label207->AutoSize = true;
			this->label207->Location = System::Drawing::Point(4, 112);
			this->label207->Name = L"label207";
			this->label207->Size = System::Drawing::Size(20, 13);
			this->label207->TabIndex = 44;
			this->label207->Text = L"dA";
			// 
			// button99
			// 
			this->button99->Location = System::Drawing::Point(273, 184);
			this->button99->Name = L"button99";
			this->button99->Size = System::Drawing::Size(46, 23);
			this->button99->TabIndex = 43;
			this->button99->Text = L"Zmieñ";
			this->button99->UseVisualStyleBackColor = true;
			// 
			// textBox135
			// 
			this->textBox135->Location = System::Drawing::Point(101, 186);
			this->textBox135->Name = L"textBox135";
			this->textBox135->Size = System::Drawing::Size(166, 20);
			this->textBox135->TabIndex = 42;
			// 
			// label208
			// 
			this->label208->AutoSize = true;
			this->label208->Location = System::Drawing::Point(3, 189);
			this->label208->Name = L"label208";
			this->label208->Size = System::Drawing::Size(49, 13);
			this->label208->TabIndex = 41;
			this->label208->Text = L"Wy_Min:";
			// 
			// button100
			// 
			this->button100->Location = System::Drawing::Point(273, 158);
			this->button100->Name = L"button100";
			this->button100->Size = System::Drawing::Size(46, 23);
			this->button100->TabIndex = 40;
			this->button100->Text = L"Zmieñ";
			this->button100->UseVisualStyleBackColor = true;
			// 
			// textBox136
			// 
			this->textBox136->Location = System::Drawing::Point(101, 160);
			this->textBox136->Name = L"textBox136";
			this->textBox136->Size = System::Drawing::Size(166, 20);
			this->textBox136->TabIndex = 39;
			// 
			// label209
			// 
			this->label209->AutoSize = true;
			this->label209->Location = System::Drawing::Point(3, 163);
			this->label209->Name = L"label209";
			this->label209->Size = System::Drawing::Size(52, 13);
			this->label209->TabIndex = 38;
			this->label209->Text = L"Wy_Max:";
			// 
			// button101
			// 
			this->button101->Location = System::Drawing::Point(196, 253);
			this->button101->Name = L"button101";
			this->button101->Size = System::Drawing::Size(123, 23);
			this->button101->TabIndex = 37;
			this->button101->Text = L"Zmieñ wszystko";
			this->button101->UseVisualStyleBackColor = true;
			// 
			// button102
			// 
			this->button102->Location = System::Drawing::Point(273, 82);
			this->button102->Name = L"button102";
			this->button102->Size = System::Drawing::Size(46, 23);
			this->button102->TabIndex = 16;
			this->button102->Text = L"Zmieñ";
			this->button102->UseVisualStyleBackColor = true;
			// 
			// textBox137
			// 
			this->textBox137->Location = System::Drawing::Point(101, 84);
			this->textBox137->Name = L"textBox137";
			this->textBox137->Size = System::Drawing::Size(166, 20);
			this->textBox137->TabIndex = 15;
			// 
			// label210
			// 
			this->label210->AutoSize = true;
			this->label210->Location = System::Drawing::Point(3, 87);
			this->label210->Name = L"label210";
			this->label210->Size = System::Drawing::Size(27, 13);
			this->label210->TabIndex = 14;
			this->label210->Text = L"alfa:";
			// 
			// button103
			// 
			this->button103->Location = System::Drawing::Point(273, 56);
			this->button103->Name = L"button103";
			this->button103->Size = System::Drawing::Size(46, 23);
			this->button103->TabIndex = 13;
			this->button103->Text = L"Zmieñ";
			this->button103->UseVisualStyleBackColor = true;
			// 
			// textBox138
			// 
			this->textBox138->Location = System::Drawing::Point(101, 58);
			this->textBox138->Name = L"textBox138";
			this->textBox138->Size = System::Drawing::Size(166, 20);
			this->textBox138->TabIndex = 12;
			// 
			// label211
			// 
			this->label211->AutoSize = true;
			this->label211->Location = System::Drawing::Point(3, 61);
			this->label211->Name = L"label211";
			this->label211->Size = System::Drawing::Size(16, 13);
			this->label211->TabIndex = 11;
			this->label211->Text = L"?:";
			// 
			// button104
			// 
			this->button104->Location = System::Drawing::Point(273, 30);
			this->button104->Name = L"button104";
			this->button104->Size = System::Drawing::Size(46, 23);
			this->button104->TabIndex = 10;
			this->button104->Text = L"Zmieñ";
			this->button104->UseVisualStyleBackColor = true;
			// 
			// textBox139
			// 
			this->textBox139->Location = System::Drawing::Point(101, 32);
			this->textBox139->Name = L"textBox139";
			this->textBox139->Size = System::Drawing::Size(166, 20);
			this->textBox139->TabIndex = 9;
			// 
			// label212
			// 
			this->label212->AutoSize = true;
			this->label212->Location = System::Drawing::Point(3, 35);
			this->label212->Name = L"label212";
			this->label212->Size = System::Drawing::Size(16, 13);
			this->label212->TabIndex = 8;
			this->label212->Text = L"L:";
			// 
			// button105
			// 
			this->button105->Location = System::Drawing::Point(273, 4);
			this->button105->Name = L"button105";
			this->button105->Size = System::Drawing::Size(46, 23);
			this->button105->TabIndex = 7;
			this->button105->Text = L"Zmieñ";
			this->button105->UseVisualStyleBackColor = true;
			// 
			// textBox140
			// 
			this->textBox140->Location = System::Drawing::Point(101, 6);
			this->textBox140->Name = L"textBox140";
			this->textBox140->Size = System::Drawing::Size(166, 20);
			this->textBox140->TabIndex = 6;
			// 
			// label213
			// 
			this->label213->AutoSize = true;
			this->label213->Location = System::Drawing::Point(3, 9);
			this->label213->Name = L"label213";
			this->label213->Size = System::Drawing::Size(18, 13);
			this->label213->TabIndex = 5;
			this->label213->Text = L"H:";
			// 
			// tabPage74
			// 
			this->tabPage74->Controls->Add(this->button106);
			this->tabPage74->Controls->Add(this->groupBox24);
			this->tabPage74->Controls->Add(this->tabControl19);
			this->tabPage74->Location = System::Drawing::Point(4, 22);
			this->tabPage74->Name = L"tabPage74";
			this->tabPage74->Padding = System::Windows::Forms::Padding(3);
			this->tabPage74->Size = System::Drawing::Size(325, 282);
			this->tabPage74->TabIndex = 1;
			this->tabPage74->Text = L"Zmiana wymuszenia";
			this->tabPage74->UseVisualStyleBackColor = true;
			// 
			// button106
			// 
			this->button106->Location = System::Drawing::Point(244, 253);
			this->button106->Name = L"button106";
			this->button106->Size = System::Drawing::Size(75, 23);
			this->button106->TabIndex = 9;
			this->button106->Text = L"Zmieñ";
			this->button106->UseVisualStyleBackColor = true;
			// 
			// groupBox24
			// 
			this->groupBox24->Controls->Add(this->checkBox32);
			this->groupBox24->Controls->Add(this->checkBox33);
			this->groupBox24->Controls->Add(this->checkBox34);
			this->groupBox24->Controls->Add(this->checkBox35);
			this->groupBox24->Controls->Add(this->checkBox36);
			this->groupBox24->Controls->Add(this->checkBox37);
			this->groupBox24->ForeColor = System::Drawing::Color::Black;
			this->groupBox24->Location = System::Drawing::Point(3, 8);
			this->groupBox24->Name = L"groupBox24";
			this->groupBox24->Size = System::Drawing::Size(316, 89);
			this->groupBox24->TabIndex = 7;
			this->groupBox24->TabStop = false;
			this->groupBox24->Text = L"Sk³adowe wymuszenia";
			// 
			// checkBox32
			// 
			this->checkBox32->AutoSize = true;
			this->checkBox32->Location = System::Drawing::Point(6, 19);
			this->checkBox32->Name = L"checkBox32";
			this->checkBox32->Size = System::Drawing::Size(51, 17);
			this->checkBox32->TabIndex = 1;
			this->checkBox32->Text = L"Skok";
			this->checkBox32->UseVisualStyleBackColor = true;
			// 
			// checkBox33
			// 
			this->checkBox33->AutoSize = true;
			this->checkBox33->Location = System::Drawing::Point(128, 65);
			this->checkBox33->Name = L"checkBox33";
			this->checkBox33->Size = System::Drawing::Size(52, 17);
			this->checkBox33->TabIndex = 6;
			this->checkBox33->Text = L"Szum";
			this->checkBox33->UseVisualStyleBackColor = true;
			// 
			// checkBox34
			// 
			this->checkBox34->AutoSize = true;
			this->checkBox34->Location = System::Drawing::Point(6, 42);
			this->checkBox34->Name = L"checkBox34";
			this->checkBox34->Size = System::Drawing::Size(108, 17);
			this->checkBox34->TabIndex = 2;
			this->checkBox34->Text = L"Impuls Kronekera";
			this->checkBox34->UseVisualStyleBackColor = true;
			// 
			// checkBox35
			// 
			this->checkBox35->AutoSize = true;
			this->checkBox35->Location = System::Drawing::Point(128, 42);
			this->checkBox35->Name = L"checkBox35";
			this->checkBox35->Size = System::Drawing::Size(59, 17);
			this->checkBox35->TabIndex = 5;
			this->checkBox35->Text = L"Trójk¹t";
			this->checkBox35->UseVisualStyleBackColor = true;
			// 
			// checkBox36
			// 
			this->checkBox36->AutoSize = true;
			this->checkBox36->Location = System::Drawing::Point(6, 65);
			this->checkBox36->Name = L"checkBox36";
			this->checkBox36->Size = System::Drawing::Size(71, 17);
			this->checkBox36->TabIndex = 3;
			this->checkBox36->Text = L"Prostok¹t";
			this->checkBox36->UseVisualStyleBackColor = true;
			// 
			// checkBox37
			// 
			this->checkBox37->AutoSize = true;
			this->checkBox37->Location = System::Drawing::Point(128, 19);
			this->checkBox37->Name = L"checkBox37";
			this->checkBox37->Size = System::Drawing::Size(52, 17);
			this->checkBox37->TabIndex = 4;
			this->checkBox37->Text = L"Sinus";
			this->checkBox37->UseVisualStyleBackColor = true;
			// 
			// tabControl19
			// 
			this->tabControl19->Controls->Add(this->tabPage75);
			this->tabControl19->Controls->Add(this->tabPage76);
			this->tabControl19->Controls->Add(this->tabPage77);
			this->tabControl19->Controls->Add(this->tabPage78);
			this->tabControl19->Controls->Add(this->tabPage79);
			this->tabControl19->Location = System::Drawing::Point(2, 103);
			this->tabControl19->Multiline = true;
			this->tabControl19->Name = L"tabControl19";
			this->tabControl19->SelectedIndex = 0;
			this->tabControl19->Size = System::Drawing::Size(316, 112);
			this->tabControl19->TabIndex = 8;
			// 
			// tabPage75
			// 
			this->tabPage75->Controls->Add(this->textBox141);
			this->tabPage75->Controls->Add(this->label214);
			this->tabPage75->Location = System::Drawing::Point(4, 22);
			this->tabPage75->Name = L"tabPage75";
			this->tabPage75->Padding = System::Windows::Forms::Padding(3);
			this->tabPage75->Size = System::Drawing::Size(308, 86);
			this->tabPage75->TabIndex = 0;
			this->tabPage75->Text = L"Skok";
			this->tabPage75->UseVisualStyleBackColor = true;
			// 
			// textBox141
			// 
			this->textBox141->Location = System::Drawing::Point(91, 6);
			this->textBox141->Name = L"textBox141";
			this->textBox141->Size = System::Drawing::Size(99, 20);
			this->textBox141->TabIndex = 1;
			// 
			// label214
			// 
			this->label214->AutoSize = true;
			this->label214->Location = System::Drawing::Point(6, 9);
			this->label214->Name = L"label214";
			this->label214->Size = System::Drawing::Size(79, 13);
			this->label214->TabIndex = 0;
			this->label214->Text = L"wartoœæ skoku:";
			// 
			// tabPage76
			// 
			this->tabPage76->Controls->Add(this->textBox142);
			this->tabPage76->Controls->Add(this->label215);
			this->tabPage76->Controls->Add(this->textBox143);
			this->tabPage76->Controls->Add(this->label216);
			this->tabPage76->Controls->Add(this->textBox144);
			this->tabPage76->Controls->Add(this->label217);
			this->tabPage76->Location = System::Drawing::Point(4, 22);
			this->tabPage76->Name = L"tabPage76";
			this->tabPage76->Padding = System::Windows::Forms::Padding(3);
			this->tabPage76->Size = System::Drawing::Size(308, 86);
			this->tabPage76->TabIndex = 1;
			this->tabPage76->Text = L"Prostok¹t";
			this->tabPage76->UseVisualStyleBackColor = true;
			// 
			// textBox142
			// 
			this->textBox142->Location = System::Drawing::Point(79, 58);
			this->textBox142->Name = L"textBox142";
			this->textBox142->Size = System::Drawing::Size(111, 20);
			this->textBox142->TabIndex = 7;
			// 
			// label215
			// 
			this->label215->AutoSize = true;
			this->label215->Location = System::Drawing::Point(6, 61);
			this->label215->Name = L"label215";
			this->label215->Size = System::Drawing::Size(67, 13);
			this->label215->TabIndex = 6;
			this->label215->Text = L"wype³nienie:";
			// 
			// textBox143
			// 
			this->textBox143->Location = System::Drawing::Point(79, 32);
			this->textBox143->Name = L"textBox143";
			this->textBox143->Size = System::Drawing::Size(111, 20);
			this->textBox143->TabIndex = 5;
			// 
			// label216
			// 
			this->label216->AutoSize = true;
			this->label216->Location = System::Drawing::Point(6, 35);
			this->label216->Name = L"label216";
			this->label216->Size = System::Drawing::Size(36, 13);
			this->label216->TabIndex = 4;
			this->label216->Text = L"okres:";
			// 
			// textBox144
			// 
			this->textBox144->Location = System::Drawing::Point(79, 6);
			this->textBox144->Name = L"textBox144";
			this->textBox144->Size = System::Drawing::Size(111, 20);
			this->textBox144->TabIndex = 3;
			// 
			// label217
			// 
			this->label217->AutoSize = true;
			this->label217->Location = System::Drawing::Point(6, 9);
			this->label217->Name = L"label217";
			this->label217->Size = System::Drawing::Size(55, 13);
			this->label217->TabIndex = 2;
			this->label217->Text = L"amplituda:";
			// 
			// tabPage77
			// 
			this->tabPage77->Controls->Add(this->textBox145);
			this->tabPage77->Controls->Add(this->label218);
			this->tabPage77->Controls->Add(this->textBox146);
			this->tabPage77->Controls->Add(this->label219);
			this->tabPage77->Location = System::Drawing::Point(4, 22);
			this->tabPage77->Name = L"tabPage77";
			this->tabPage77->Size = System::Drawing::Size(308, 86);
			this->tabPage77->TabIndex = 2;
			this->tabPage77->Text = L"Sinus";
			this->tabPage77->UseVisualStyleBackColor = true;
			// 
			// textBox145
			// 
			this->textBox145->Location = System::Drawing::Point(67, 32);
			this->textBox145->Name = L"textBox145";
			this->textBox145->Size = System::Drawing::Size(123, 20);
			this->textBox145->TabIndex = 9;
			// 
			// label218
			// 
			this->label218->AutoSize = true;
			this->label218->Location = System::Drawing::Point(6, 35);
			this->label218->Name = L"label218";
			this->label218->Size = System::Drawing::Size(36, 13);
			this->label218->TabIndex = 8;
			this->label218->Text = L"okres:";
			// 
			// textBox146
			// 
			this->textBox146->Location = System::Drawing::Point(67, 6);
			this->textBox146->Name = L"textBox146";
			this->textBox146->Size = System::Drawing::Size(123, 20);
			this->textBox146->TabIndex = 7;
			// 
			// label219
			// 
			this->label219->AutoSize = true;
			this->label219->Location = System::Drawing::Point(6, 9);
			this->label219->Name = L"label219";
			this->label219->Size = System::Drawing::Size(55, 13);
			this->label219->TabIndex = 6;
			this->label219->Text = L"amplituda:";
			// 
			// tabPage78
			// 
			this->tabPage78->Controls->Add(this->textBox147);
			this->tabPage78->Controls->Add(this->label220);
			this->tabPage78->Controls->Add(this->textBox148);
			this->tabPage78->Controls->Add(this->label221);
			this->tabPage78->Location = System::Drawing::Point(4, 22);
			this->tabPage78->Name = L"tabPage78";
			this->tabPage78->Size = System::Drawing::Size(308, 86);
			this->tabPage78->TabIndex = 3;
			this->tabPage78->Text = L"Trójk¹t";
			this->tabPage78->UseVisualStyleBackColor = true;
			// 
			// textBox147
			// 
			this->textBox147->Location = System::Drawing::Point(67, 32);
			this->textBox147->Name = L"textBox147";
			this->textBox147->Size = System::Drawing::Size(123, 20);
			this->textBox147->TabIndex = 13;
			// 
			// label220
			// 
			this->label220->AutoSize = true;
			this->label220->Location = System::Drawing::Point(6, 35);
			this->label220->Name = L"label220";
			this->label220->Size = System::Drawing::Size(36, 13);
			this->label220->TabIndex = 12;
			this->label220->Text = L"okres:";
			// 
			// textBox148
			// 
			this->textBox148->Location = System::Drawing::Point(67, 7);
			this->textBox148->Name = L"textBox148";
			this->textBox148->Size = System::Drawing::Size(123, 20);
			this->textBox148->TabIndex = 11;
			// 
			// label221
			// 
			this->label221->AutoSize = true;
			this->label221->Location = System::Drawing::Point(6, 9);
			this->label221->Name = L"label221";
			this->label221->Size = System::Drawing::Size(55, 13);
			this->label221->TabIndex = 10;
			this->label221->Text = L"amplituda:";
			// 
			// tabPage79
			// 
			this->tabPage79->Controls->Add(this->textBox149);
			this->tabPage79->Controls->Add(this->label222);
			this->tabPage79->Controls->Add(this->textBox150);
			this->tabPage79->Controls->Add(this->label223);
			this->tabPage79->Location = System::Drawing::Point(4, 22);
			this->tabPage79->Name = L"tabPage79";
			this->tabPage79->Size = System::Drawing::Size(308, 86);
			this->tabPage79->TabIndex = 4;
			this->tabPage79->Text = L"Szum";
			this->tabPage79->UseVisualStyleBackColor = true;
			// 
			// textBox149
			// 
			this->textBox149->Location = System::Drawing::Point(82, 32);
			this->textBox149->Name = L"textBox149";
			this->textBox149->Size = System::Drawing::Size(108, 20);
			this->textBox149->TabIndex = 13;
			// 
			// label222
			// 
			this->label222->AutoSize = true;
			this->label222->Location = System::Drawing::Point(6, 35);
			this->label222->Name = L"label222";
			this->label222->Size = System::Drawing::Size(73, 13);
			this->label222->TabIndex = 12;
			this->label222->Text = L"liczba próbek:";
			// 
			// textBox150
			// 
			this->textBox150->Location = System::Drawing::Point(82, 7);
			this->textBox150->Name = L"textBox150";
			this->textBox150->Size = System::Drawing::Size(108, 20);
			this->textBox150->TabIndex = 11;
			// 
			// label223
			// 
			this->label223->AutoSize = true;
			this->label223->Location = System::Drawing::Point(6, 10);
			this->label223->Name = L"label223";
			this->label223->Size = System::Drawing::Size(55, 13);
			this->label223->TabIndex = 10;
			this->label223->Text = L"wariancja:";
			// 
			// tabPage80
			// 
			this->tabPage80->Controls->Add(this->button107);
			this->tabPage80->Controls->Add(this->button108);
			this->tabPage80->Controls->Add(this->label224);
			this->tabPage80->Controls->Add(this->textBox151);
			this->tabPage80->Controls->Add(this->textBox152);
			this->tabPage80->Location = System::Drawing::Point(4, 22);
			this->tabPage80->Name = L"tabPage80";
			this->tabPage80->Size = System::Drawing::Size(325, 282);
			this->tabPage80->TabIndex = 2;
			this->tabPage80->Text = L"Identyfikacja";
			this->tabPage80->UseVisualStyleBackColor = true;
			// 
			// button107
			// 
			this->button107->Location = System::Drawing::Point(247, 255);
			this->button107->Name = L"button107";
			this->button107->Size = System::Drawing::Size(75, 23);
			this->button107->TabIndex = 10;
			this->button107->Text = L"Wyczysc";
			this->button107->UseVisualStyleBackColor = true;
			// 
			// button108
			// 
			this->button108->Location = System::Drawing::Point(166, 5);
			this->button108->Name = L"button108";
			this->button108->Size = System::Drawing::Size(75, 23);
			this->button108->TabIndex = 9;
			this->button108->Text = L"Zmieñ";
			this->button108->UseVisualStyleBackColor = true;
			// 
			// label224
			// 
			this->label224->AutoSize = true;
			this->label224->Location = System::Drawing::Point(11, 10);
			this->label224->Name = L"label224";
			this->label224->Size = System::Drawing::Size(27, 13);
			this->label224->TabIndex = 8;
			this->label224->Text = L"alfa:";
			// 
			// textBox151
			// 
			this->textBox151->Location = System::Drawing::Point(3, 29);
			this->textBox151->Multiline = true;
			this->textBox151->Name = L"textBox151";
			this->textBox151->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox151->Size = System::Drawing::Size(319, 220);
			this->textBox151->TabIndex = 6;
			// 
			// textBox152
			// 
			this->textBox152->Location = System::Drawing::Point(58, 7);
			this->textBox152->Name = L"textBox152";
			this->textBox152->Size = System::Drawing::Size(102, 20);
			this->textBox152->TabIndex = 7;
			this->textBox152->Text = L"0.99";
			// 
			// groupBox25
			// 
			this->groupBox25->Controls->Add(this->button109);
			this->groupBox25->Controls->Add(this->label225);
			this->groupBox25->Controls->Add(this->textBox153);
			this->groupBox25->ForeColor = System::Drawing::Color::Black;
			this->groupBox25->Location = System::Drawing::Point(6, 6);
			this->groupBox25->Name = L"groupBox25";
			this->groupBox25->Size = System::Drawing::Size(333, 79);
			this->groupBox25->TabIndex = 17;
			this->groupBox25->TabStop = false;
			this->groupBox25->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// button109
			// 
			this->button109->Location = System::Drawing::Point(252, 45);
			this->button109->Name = L"button109";
			this->button109->Size = System::Drawing::Size(75, 23);
			this->button109->TabIndex = 2;
			this->button109->Text = L"Wczytaj";
			this->button109->UseVisualStyleBackColor = true;
			// 
			// label225
			// 
			this->label225->AutoSize = true;
			this->label225->Location = System::Drawing::Point(6, 22);
			this->label225->Name = L"label225";
			this->label225->Size = System::Drawing::Size(93, 13);
			this->label225->TabIndex = 1;
			this->label225->Text = L"Nazwa regulatora:";
			// 
			// textBox153
			// 
			this->textBox153->Location = System::Drawing::Point(105, 19);
			this->textBox153->Name = L"textBox153";
			this->textBox153->Size = System::Drawing::Size(222, 20);
			this->textBox153->TabIndex = 0;
			// 
			// tabPage81
			// 
			this->tabPage81->Controls->Add(this->button110);
			this->tabPage81->Controls->Add(this->label226);
			this->tabPage81->Controls->Add(this->textBox154);
			this->tabPage81->Controls->Add(this->button111);
			this->tabPage81->Controls->Add(this->textBox155);
			this->tabPage81->Controls->Add(this->checkBox38);
			this->tabPage81->Location = System::Drawing::Point(4, 22);
			this->tabPage81->Name = L"tabPage81";
			this->tabPage81->Size = System::Drawing::Size(345, 494);
			this->tabPage81->TabIndex = 4;
			this->tabPage81->Text = L"Identyfikacja";
			this->tabPage81->UseVisualStyleBackColor = true;
			// 
			// button110
			// 
			this->button110->Location = System::Drawing::Point(158, 24);
			this->button110->Name = L"button110";
			this->button110->Size = System::Drawing::Size(75, 23);
			this->button110->TabIndex = 5;
			this->button110->Text = L"Zmieñ";
			this->button110->UseVisualStyleBackColor = true;
			// 
			// label226
			// 
			this->label226->AutoSize = true;
			this->label226->Location = System::Drawing::Point(3, 29);
			this->label226->Name = L"label226";
			this->label226->Size = System::Drawing::Size(27, 13);
			this->label226->TabIndex = 4;
			this->label226->Text = L"alfa:";
			// 
			// textBox154
			// 
			this->textBox154->Location = System::Drawing::Point(50, 26);
			this->textBox154->Name = L"textBox154";
			this->textBox154->Size = System::Drawing::Size(102, 20);
			this->textBox154->TabIndex = 3;
			this->textBox154->Text = L"0.99";
			// 
			// button111
			// 
			this->button111->Location = System::Drawing::Point(267, 468);
			this->button111->Name = L"button111";
			this->button111->Size = System::Drawing::Size(75, 23);
			this->button111->TabIndex = 2;
			this->button111->Text = L"Wyczysc";
			this->button111->UseVisualStyleBackColor = true;
			// 
			// textBox155
			// 
			this->textBox155->Location = System::Drawing::Point(3, 52);
			this->textBox155->Multiline = true;
			this->textBox155->Name = L"textBox155";
			this->textBox155->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox155->Size = System::Drawing::Size(339, 410);
			this->textBox155->TabIndex = 1;
			// 
			// checkBox38
			// 
			this->checkBox38->AutoSize = true;
			this->checkBox38->Location = System::Drawing::Point(3, 3);
			this->checkBox38->Name = L"checkBox38";
			this->checkBox38->Size = System::Drawing::Size(147, 17);
			this->checkBox38->TabIndex = 0;
			this->checkBox38->Text = L"Identyfikuj obiekt ARMAX";
			this->checkBox38->UseVisualStyleBackColor = true;
			// 
			// tabPage82
			// 
			this->tabPage82->Controls->Add(this->ZmienNastawy);
			this->tabPage82->Controls->Add(this->tabControl7);
			this->tabPage82->Location = System::Drawing::Point(4, 22);
			this->tabPage82->Name = L"tabPage82";
			this->tabPage82->Padding = System::Windows::Forms::Padding(3);
			this->tabPage82->Size = System::Drawing::Size(438, 494);
			this->tabPage82->TabIndex = 5;
			this->tabPage82->Text = L"Adaptacyjny PID";
			this->tabPage82->UseVisualStyleBackColor = true;
			// 
			// ZmienNastawy
			// 
			this->ZmienNastawy->AutoSize = true;
			this->ZmienNastawy->Location = System::Drawing::Point(15, 9);
			this->ZmienNastawy->Name = L"ZmienNastawy";
			this->ZmienNastawy->Size = System::Drawing::Size(87, 17);
			this->ZmienNastawy->TabIndex = 22;
			this->ZmienNastawy->Text = L"Eksperyment";
			this->ZmienNastawy->UseVisualStyleBackColor = true;
			this->ZmienNastawy->Click += gcnew System::EventHandler(this, &Form1::ekspAPID_Changed);
			// 
			// tabControl7
			// 
			this->tabControl7->Controls->Add(this->tabPage83);
			this->tabControl7->Controls->Add(this->tabPage84);
			this->tabControl7->Location = System::Drawing::Point(11, 47);
			this->tabControl7->Name = L"tabControl7";
			this->tabControl7->SelectedIndex = 0;
			this->tabControl7->Size = System::Drawing::Size(417, 308);
			this->tabControl7->TabIndex = 21;
			// 
			// tabPage83
			// 
			this->tabPage83->Controls->Add(this->zmienWyMinAPID);
			this->tabPage83->Controls->Add(this->wyMinAPID);
			this->tabPage83->Controls->Add(this->label227);
			this->tabPage83->Controls->Add(this->zmienWyMaxAPID);
			this->tabPage83->Controls->Add(this->wyMaxAPID);
			this->tabPage83->Controls->Add(this->label228);
			this->tabPage83->Controls->Add(this->zmienWszystkoAPID);
			this->tabPage83->Controls->Add(this->zmienOnAPID);
			this->tabPage83->Controls->Add(this->onAPID);
			this->tabPage83->Controls->Add(this->label232);
			this->tabPage83->Controls->Add(this->zmienTdAPID);
			this->tabPage83->Controls->Add(this->tdAPID);
			this->tabPage83->Controls->Add(this->label233);
			this->tabPage83->Controls->Add(this->zmienTiAPID);
			this->tabPage83->Controls->Add(this->tiAPID);
			this->tabPage83->Controls->Add(this->label234);
			this->tabPage83->Controls->Add(this->zmienKAPID);
			this->tabPage83->Controls->Add(this->kAPID);
			this->tabPage83->Controls->Add(this->label235);
			this->tabPage83->Location = System::Drawing::Point(4, 22);
			this->tabPage83->Name = L"tabPage83";
			this->tabPage83->Padding = System::Windows::Forms::Padding(3);
			this->tabPage83->Size = System::Drawing::Size(409, 282);
			this->tabPage83->TabIndex = 0;
			this->tabPage83->Text = L"Zmiana parametrów";
			this->tabPage83->UseVisualStyleBackColor = true;
			// 
			// zmienWyMinAPID
			// 
			this->zmienWyMinAPID->Location = System::Drawing::Point(273, 135);
			this->zmienWyMinAPID->Name = L"zmienWyMinAPID";
			this->zmienWyMinAPID->Size = System::Drawing::Size(46, 23);
			this->zmienWyMinAPID->TabIndex = 43;
			this->zmienWyMinAPID->Text = L"Zmieñ";
			this->zmienWyMinAPID->UseVisualStyleBackColor = true;
			this->zmienWyMinAPID->Click += gcnew System::EventHandler(this, &Form1::zmienWyMinAPID_Click);
			// 
			// wyMinAPID
			// 
			this->wyMinAPID->Location = System::Drawing::Point(101, 137);
			this->wyMinAPID->Name = L"wyMinAPID";
			this->wyMinAPID->Size = System::Drawing::Size(166, 20);
			this->wyMinAPID->TabIndex = 42;
			// 
			// label227
			// 
			this->label227->AutoSize = true;
			this->label227->Location = System::Drawing::Point(3, 140);
			this->label227->Name = L"label227";
			this->label227->Size = System::Drawing::Size(49, 13);
			this->label227->TabIndex = 41;
			this->label227->Text = L"Wy_Min:";
			// 
			// zmienWyMaxAPID
			// 
			this->zmienWyMaxAPID->Location = System::Drawing::Point(273, 109);
			this->zmienWyMaxAPID->Name = L"zmienWyMaxAPID";
			this->zmienWyMaxAPID->Size = System::Drawing::Size(46, 23);
			this->zmienWyMaxAPID->TabIndex = 40;
			this->zmienWyMaxAPID->Text = L"Zmieñ";
			this->zmienWyMaxAPID->UseVisualStyleBackColor = true;
			this->zmienWyMaxAPID->Click += gcnew System::EventHandler(this, &Form1::zmienWyMaxAPID_Click);
			// 
			// wyMaxAPID
			// 
			this->wyMaxAPID->Location = System::Drawing::Point(101, 111);
			this->wyMaxAPID->Name = L"wyMaxAPID";
			this->wyMaxAPID->Size = System::Drawing::Size(166, 20);
			this->wyMaxAPID->TabIndex = 39;
			// 
			// label228
			// 
			this->label228->AutoSize = true;
			this->label228->Location = System::Drawing::Point(3, 114);
			this->label228->Name = L"label228";
			this->label228->Size = System::Drawing::Size(52, 13);
			this->label228->TabIndex = 38;
			this->label228->Text = L"Wy_Max:";
			// 
			// zmienWszystkoAPID
			// 
			this->zmienWszystkoAPID->Location = System::Drawing::Point(196, 176);
			this->zmienWszystkoAPID->Name = L"zmienWszystkoAPID";
			this->zmienWszystkoAPID->Size = System::Drawing::Size(123, 23);
			this->zmienWszystkoAPID->TabIndex = 37;
			this->zmienWszystkoAPID->Text = L"Zmieñ wszystko";
			this->zmienWszystkoAPID->UseVisualStyleBackColor = true;
			this->zmienWszystkoAPID->Click += gcnew System::EventHandler(this, &Form1::zmienWszystkoAPID_Click);
			// 
			// zmienOnAPID
			// 
			this->zmienOnAPID->Location = System::Drawing::Point(273, 82);
			this->zmienOnAPID->Name = L"zmienOnAPID";
			this->zmienOnAPID->Size = System::Drawing::Size(46, 23);
			this->zmienOnAPID->TabIndex = 16;
			this->zmienOnAPID->Text = L"Zmieñ";
			this->zmienOnAPID->UseVisualStyleBackColor = true;
			this->zmienOnAPID->Click += gcnew System::EventHandler(this, &Form1::zmienOnAPID_Click);
			// 
			// onAPID
			// 
			this->onAPID->Location = System::Drawing::Point(101, 84);
			this->onAPID->Name = L"onAPID";
			this->onAPID->Size = System::Drawing::Size(166, 20);
			this->onAPID->TabIndex = 15;
			// 
			// label232
			// 
			this->label232->AutoSize = true;
			this->label232->Location = System::Drawing::Point(3, 87);
			this->label232->Name = L"label232";
			this->label232->Size = System::Drawing::Size(26, 13);
			this->label232->TabIndex = 14;
			this->label232->Text = L"ON:";
			// 
			// zmienTdAPID
			// 
			this->zmienTdAPID->Location = System::Drawing::Point(273, 56);
			this->zmienTdAPID->Name = L"zmienTdAPID";
			this->zmienTdAPID->Size = System::Drawing::Size(46, 23);
			this->zmienTdAPID->TabIndex = 13;
			this->zmienTdAPID->Text = L"Zmieñ";
			this->zmienTdAPID->UseVisualStyleBackColor = true;
			this->zmienTdAPID->Click += gcnew System::EventHandler(this, &Form1::zmienTdAPID_Click);
			// 
			// tdAPID
			// 
			this->tdAPID->Location = System::Drawing::Point(101, 58);
			this->tdAPID->Name = L"tdAPID";
			this->tdAPID->Size = System::Drawing::Size(166, 20);
			this->tdAPID->TabIndex = 12;
			// 
			// label233
			// 
			this->label233->AutoSize = true;
			this->label233->Location = System::Drawing::Point(3, 61);
			this->label233->Name = L"label233";
			this->label233->Size = System::Drawing::Size(23, 13);
			this->label233->TabIndex = 11;
			this->label233->Text = L"Td:";
			// 
			// zmienTiAPID
			// 
			this->zmienTiAPID->Location = System::Drawing::Point(273, 30);
			this->zmienTiAPID->Name = L"zmienTiAPID";
			this->zmienTiAPID->Size = System::Drawing::Size(46, 23);
			this->zmienTiAPID->TabIndex = 10;
			this->zmienTiAPID->Text = L"Zmieñ";
			this->zmienTiAPID->UseVisualStyleBackColor = true;
			this->zmienTiAPID->Click += gcnew System::EventHandler(this, &Form1::zmienTiAPID_Click);
			// 
			// tiAPID
			// 
			this->tiAPID->Location = System::Drawing::Point(101, 32);
			this->tiAPID->Name = L"tiAPID";
			this->tiAPID->Size = System::Drawing::Size(166, 20);
			this->tiAPID->TabIndex = 9;
			// 
			// label234
			// 
			this->label234->AutoSize = true;
			this->label234->Location = System::Drawing::Point(3, 35);
			this->label234->Name = L"label234";
			this->label234->Size = System::Drawing::Size(19, 13);
			this->label234->TabIndex = 8;
			this->label234->Text = L"Ti:";
			// 
			// zmienKAPID
			// 
			this->zmienKAPID->Location = System::Drawing::Point(273, 4);
			this->zmienKAPID->Name = L"zmienKAPID";
			this->zmienKAPID->Size = System::Drawing::Size(46, 23);
			this->zmienKAPID->TabIndex = 7;
			this->zmienKAPID->Text = L"Zmieñ";
			this->zmienKAPID->UseVisualStyleBackColor = true;
			this->zmienKAPID->Click += gcnew System::EventHandler(this, &Form1::zmienKAPID_Click);
			// 
			// kAPID
			// 
			this->kAPID->Location = System::Drawing::Point(101, 6);
			this->kAPID->Name = L"kAPID";
			this->kAPID->Size = System::Drawing::Size(166, 20);
			this->kAPID->TabIndex = 6;
			// 
			// label235
			// 
			this->label235->AutoSize = true;
			this->label235->Location = System::Drawing::Point(3, 9);
			this->label235->Name = L"label235";
			this->label235->Size = System::Drawing::Size(16, 13);
			this->label235->TabIndex = 5;
			this->label235->Text = L"k:";
			// 
			// tabPage84
			// 
			this->tabPage84->Controls->Add(this->zmienWymuszenieAPID);
			this->tabPage84->Controls->Add(this->groupBox26);
			this->tabPage84->Controls->Add(this->tabPage999);
			this->tabPage84->Location = System::Drawing::Point(4, 22);
			this->tabPage84->Name = L"tabPage84";
			this->tabPage84->Padding = System::Windows::Forms::Padding(3);
			this->tabPage84->Size = System::Drawing::Size(409, 282);
			this->tabPage84->TabIndex = 1;
			this->tabPage84->Text = L"Zmiana wymuszenia";
			this->tabPage84->UseVisualStyleBackColor = true;
			// 
			// zmienWymuszenieAPID
			// 
			this->zmienWymuszenieAPID->Location = System::Drawing::Point(244, 253);
			this->zmienWymuszenieAPID->Name = L"zmienWymuszenieAPID";
			this->zmienWymuszenieAPID->Size = System::Drawing::Size(75, 23);
			this->zmienWymuszenieAPID->TabIndex = 9;
			this->zmienWymuszenieAPID->Text = L"Zmieñ";
			this->zmienWymuszenieAPID->UseVisualStyleBackColor = true;
			this->zmienWymuszenieAPID->Click += gcnew System::EventHandler(this, &Form1::zmienWymuszenieAPID_Click);
			// 
			// groupBox26
			// 
			this->groupBox26->Controls->Add(this->skokAPID);
			this->groupBox26->Controls->Add(this->szumAPID);
			this->groupBox26->Controls->Add(this->kronekerAPID);
			this->groupBox26->Controls->Add(this->trojkatAPID);
			this->groupBox26->Controls->Add(this->prostokatAPID);
			this->groupBox26->Controls->Add(this->sinusAPID);
			this->groupBox26->ForeColor = System::Drawing::Color::Black;
			this->groupBox26->Location = System::Drawing::Point(3, 8);
			this->groupBox26->Name = L"groupBox26";
			this->groupBox26->Size = System::Drawing::Size(316, 89);
			this->groupBox26->TabIndex = 7;
			this->groupBox26->TabStop = false;
			this->groupBox26->Text = L"Sk³adowe wymuszenia";
			// 
			// skokAPID
			// 
			this->skokAPID->AutoSize = true;
			this->skokAPID->Location = System::Drawing::Point(6, 19);
			this->skokAPID->Name = L"skokAPID";
			this->skokAPID->Size = System::Drawing::Size(51, 17);
			this->skokAPID->TabIndex = 1;
			this->skokAPID->Text = L"Skok";
			this->skokAPID->UseVisualStyleBackColor = true;
			// 
			// szumAPID
			// 
			this->szumAPID->AutoSize = true;
			this->szumAPID->Location = System::Drawing::Point(128, 65);
			this->szumAPID->Name = L"szumAPID";
			this->szumAPID->Size = System::Drawing::Size(52, 17);
			this->szumAPID->TabIndex = 6;
			this->szumAPID->Text = L"Szum";
			this->szumAPID->UseVisualStyleBackColor = true;
			// 
			// kronekerAPID
			// 
			this->kronekerAPID->AutoSize = true;
			this->kronekerAPID->Location = System::Drawing::Point(6, 42);
			this->kronekerAPID->Name = L"kronekerAPID";
			this->kronekerAPID->Size = System::Drawing::Size(108, 17);
			this->kronekerAPID->TabIndex = 2;
			this->kronekerAPID->Text = L"Impuls Kronekera";
			this->kronekerAPID->UseVisualStyleBackColor = true;
			// 
			// trojkatAPID
			// 
			this->trojkatAPID->AutoSize = true;
			this->trojkatAPID->Location = System::Drawing::Point(128, 42);
			this->trojkatAPID->Name = L"trojkatAPID";
			this->trojkatAPID->Size = System::Drawing::Size(59, 17);
			this->trojkatAPID->TabIndex = 5;
			this->trojkatAPID->Text = L"Trójk¹t";
			this->trojkatAPID->UseVisualStyleBackColor = true;
			// 
			// prostokatAPID
			// 
			this->prostokatAPID->AutoSize = true;
			this->prostokatAPID->Location = System::Drawing::Point(6, 65);
			this->prostokatAPID->Name = L"prostokatAPID";
			this->prostokatAPID->Size = System::Drawing::Size(71, 17);
			this->prostokatAPID->TabIndex = 3;
			this->prostokatAPID->Text = L"Prostok¹t";
			this->prostokatAPID->UseVisualStyleBackColor = true;
			// 
			// sinusAPID
			// 
			this->sinusAPID->AutoSize = true;
			this->sinusAPID->Location = System::Drawing::Point(128, 19);
			this->sinusAPID->Name = L"sinusAPID";
			this->sinusAPID->Size = System::Drawing::Size(52, 17);
			this->sinusAPID->TabIndex = 4;
			this->sinusAPID->Text = L"Sinus";
			this->sinusAPID->UseVisualStyleBackColor = true;
			// 
			// tabPage999
			// 
			this->tabPage999->Controls->Add(this->tabPage85);
			this->tabPage999->Controls->Add(this->tabPage86);
			this->tabPage999->Controls->Add(this->tabPage87);
			this->tabPage999->Controls->Add(this->tabPage88);
			this->tabPage999->Controls->Add(this->tabPage89);
			this->tabPage999->Location = System::Drawing::Point(2, 103);
			this->tabPage999->Multiline = true;
			this->tabPage999->Name = L"tabPage999";
			this->tabPage999->SelectedIndex = 0;
			this->tabPage999->Size = System::Drawing::Size(316, 112);
			this->tabPage999->TabIndex = 8;
			// 
			// tabPage85
			// 
			this->tabPage85->Controls->Add(this->nowySkokAPID);
			this->tabPage85->Controls->Add(this->label236);
			this->tabPage85->Location = System::Drawing::Point(4, 22);
			this->tabPage85->Name = L"tabPage85";
			this->tabPage85->Padding = System::Windows::Forms::Padding(3);
			this->tabPage85->Size = System::Drawing::Size(308, 86);
			this->tabPage85->TabIndex = 0;
			this->tabPage85->Text = L"Skok";
			this->tabPage85->UseVisualStyleBackColor = true;
			// 
			// nowySkokAPID
			// 
			this->nowySkokAPID->Location = System::Drawing::Point(91, 6);
			this->nowySkokAPID->Name = L"nowySkokAPID";
			this->nowySkokAPID->Size = System::Drawing::Size(99, 20);
			this->nowySkokAPID->TabIndex = 1;
			// 
			// label236
			// 
			this->label236->AutoSize = true;
			this->label236->Location = System::Drawing::Point(6, 9);
			this->label236->Name = L"label236";
			this->label236->Size = System::Drawing::Size(79, 13);
			this->label236->TabIndex = 0;
			this->label236->Text = L"wartoœæ skoku:";
			// 
			// tabPage86
			// 
			this->tabPage86->Controls->Add(this->noweWypelnieniePAPID);
			this->tabPage86->Controls->Add(this->label237);
			this->tabPage86->Controls->Add(this->nowyOkresPAPID);
			this->tabPage86->Controls->Add(this->label238);
			this->tabPage86->Controls->Add(this->nowaAmplitudaPAPID);
			this->tabPage86->Controls->Add(this->label239);
			this->tabPage86->Location = System::Drawing::Point(4, 22);
			this->tabPage86->Name = L"tabPage86";
			this->tabPage86->Padding = System::Windows::Forms::Padding(3);
			this->tabPage86->Size = System::Drawing::Size(308, 86);
			this->tabPage86->TabIndex = 1;
			this->tabPage86->Text = L"Prostok¹t";
			this->tabPage86->UseVisualStyleBackColor = true;
			// 
			// noweWypelnieniePAPID
			// 
			this->noweWypelnieniePAPID->Location = System::Drawing::Point(79, 58);
			this->noweWypelnieniePAPID->Name = L"noweWypelnieniePAPID";
			this->noweWypelnieniePAPID->Size = System::Drawing::Size(111, 20);
			this->noweWypelnieniePAPID->TabIndex = 7;
			// 
			// label237
			// 
			this->label237->AutoSize = true;
			this->label237->Location = System::Drawing::Point(6, 61);
			this->label237->Name = L"label237";
			this->label237->Size = System::Drawing::Size(67, 13);
			this->label237->TabIndex = 6;
			this->label237->Text = L"wype³nienie:";
			// 
			// nowyOkresPAPID
			// 
			this->nowyOkresPAPID->Location = System::Drawing::Point(79, 32);
			this->nowyOkresPAPID->Name = L"nowyOkresPAPID";
			this->nowyOkresPAPID->Size = System::Drawing::Size(111, 20);
			this->nowyOkresPAPID->TabIndex = 5;
			// 
			// label238
			// 
			this->label238->AutoSize = true;
			this->label238->Location = System::Drawing::Point(6, 35);
			this->label238->Name = L"label238";
			this->label238->Size = System::Drawing::Size(36, 13);
			this->label238->TabIndex = 4;
			this->label238->Text = L"okres:";
			// 
			// nowaAmplitudaPAPID
			// 
			this->nowaAmplitudaPAPID->Location = System::Drawing::Point(79, 6);
			this->nowaAmplitudaPAPID->Name = L"nowaAmplitudaPAPID";
			this->nowaAmplitudaPAPID->Size = System::Drawing::Size(111, 20);
			this->nowaAmplitudaPAPID->TabIndex = 3;
			// 
			// label239
			// 
			this->label239->AutoSize = true;
			this->label239->Location = System::Drawing::Point(6, 9);
			this->label239->Name = L"label239";
			this->label239->Size = System::Drawing::Size(55, 13);
			this->label239->TabIndex = 2;
			this->label239->Text = L"amplituda:";
			// 
			// tabPage87
			// 
			this->tabPage87->Controls->Add(this->nowyOkresSAPID);
			this->tabPage87->Controls->Add(this->label240);
			this->tabPage87->Controls->Add(this->nowaAmplitudaSAPID);
			this->tabPage87->Controls->Add(this->label241);
			this->tabPage87->Location = System::Drawing::Point(4, 22);
			this->tabPage87->Name = L"tabPage87";
			this->tabPage87->Size = System::Drawing::Size(308, 86);
			this->tabPage87->TabIndex = 2;
			this->tabPage87->Text = L"Sinus";
			this->tabPage87->UseVisualStyleBackColor = true;
			// 
			// nowyOkresSAPID
			// 
			this->nowyOkresSAPID->Location = System::Drawing::Point(67, 32);
			this->nowyOkresSAPID->Name = L"nowyOkresSAPID";
			this->nowyOkresSAPID->Size = System::Drawing::Size(123, 20);
			this->nowyOkresSAPID->TabIndex = 9;
			// 
			// label240
			// 
			this->label240->AutoSize = true;
			this->label240->Location = System::Drawing::Point(6, 35);
			this->label240->Name = L"label240";
			this->label240->Size = System::Drawing::Size(36, 13);
			this->label240->TabIndex = 8;
			this->label240->Text = L"okres:";
			// 
			// nowaAmplitudaSAPID
			// 
			this->nowaAmplitudaSAPID->Location = System::Drawing::Point(67, 6);
			this->nowaAmplitudaSAPID->Name = L"nowaAmplitudaSAPID";
			this->nowaAmplitudaSAPID->Size = System::Drawing::Size(123, 20);
			this->nowaAmplitudaSAPID->TabIndex = 7;
			// 
			// label241
			// 
			this->label241->AutoSize = true;
			this->label241->Location = System::Drawing::Point(6, 9);
			this->label241->Name = L"label241";
			this->label241->Size = System::Drawing::Size(55, 13);
			this->label241->TabIndex = 6;
			this->label241->Text = L"amplituda:";
			// 
			// tabPage88
			// 
			this->tabPage88->Controls->Add(this->nowyOkresTAPID);
			this->tabPage88->Controls->Add(this->label242);
			this->tabPage88->Controls->Add(this->nowaAmplitudaTAPID);
			this->tabPage88->Controls->Add(this->label243);
			this->tabPage88->Location = System::Drawing::Point(4, 22);
			this->tabPage88->Name = L"tabPage88";
			this->tabPage88->Size = System::Drawing::Size(308, 86);
			this->tabPage88->TabIndex = 3;
			this->tabPage88->Text = L"Trójk¹t";
			this->tabPage88->UseVisualStyleBackColor = true;
			// 
			// nowyOkresTAPID
			// 
			this->nowyOkresTAPID->Location = System::Drawing::Point(67, 32);
			this->nowyOkresTAPID->Name = L"nowyOkresTAPID";
			this->nowyOkresTAPID->Size = System::Drawing::Size(123, 20);
			this->nowyOkresTAPID->TabIndex = 13;
			// 
			// label242
			// 
			this->label242->AutoSize = true;
			this->label242->Location = System::Drawing::Point(6, 35);
			this->label242->Name = L"label242";
			this->label242->Size = System::Drawing::Size(36, 13);
			this->label242->TabIndex = 12;
			this->label242->Text = L"okres:";
			// 
			// nowaAmplitudaTAPID
			// 
			this->nowaAmplitudaTAPID->Location = System::Drawing::Point(67, 7);
			this->nowaAmplitudaTAPID->Name = L"nowaAmplitudaTAPID";
			this->nowaAmplitudaTAPID->Size = System::Drawing::Size(123, 20);
			this->nowaAmplitudaTAPID->TabIndex = 11;
			// 
			// label243
			// 
			this->label243->AutoSize = true;
			this->label243->Location = System::Drawing::Point(6, 9);
			this->label243->Name = L"label243";
			this->label243->Size = System::Drawing::Size(55, 13);
			this->label243->TabIndex = 10;
			this->label243->Text = L"amplituda:";
			// 
			// tabPage89
			// 
			this->tabPage89->Controls->Add(this->nowaLiczbaProbekAPID);
			this->tabPage89->Controls->Add(this->label244);
			this->tabPage89->Controls->Add(this->nowaWariancjaAPID);
			this->tabPage89->Controls->Add(this->label245);
			this->tabPage89->Location = System::Drawing::Point(4, 22);
			this->tabPage89->Name = L"tabPage89";
			this->tabPage89->Size = System::Drawing::Size(308, 86);
			this->tabPage89->TabIndex = 4;
			this->tabPage89->Text = L"Szum";
			this->tabPage89->UseVisualStyleBackColor = true;
			// 
			// nowaLiczbaProbekAPID
			// 
			this->nowaLiczbaProbekAPID->Location = System::Drawing::Point(82, 32);
			this->nowaLiczbaProbekAPID->Name = L"nowaLiczbaProbekAPID";
			this->nowaLiczbaProbekAPID->Size = System::Drawing::Size(108, 20);
			this->nowaLiczbaProbekAPID->TabIndex = 13;
			// 
			// label244
			// 
			this->label244->AutoSize = true;
			this->label244->Location = System::Drawing::Point(6, 35);
			this->label244->Name = L"label244";
			this->label244->Size = System::Drawing::Size(73, 13);
			this->label244->TabIndex = 12;
			this->label244->Text = L"liczba próbek:";
			// 
			// nowaWariancjaAPID
			// 
			this->nowaWariancjaAPID->Location = System::Drawing::Point(82, 7);
			this->nowaWariancjaAPID->Name = L"nowaWariancjaAPID";
			this->nowaWariancjaAPID->Size = System::Drawing::Size(108, 20);
			this->nowaWariancjaAPID->TabIndex = 11;
			// 
			// label245
			// 
			this->label245->AutoSize = true;
			this->label245->Location = System::Drawing::Point(6, 10);
			this->label245->Name = L"label245";
			this->label245->Size = System::Drawing::Size(55, 13);
			this->label245->TabIndex = 10;
			this->label245->Text = L"wariancja:";
			// 
			// tabPage19
			// 
			this->tabPage19->Controls->Add(this->zmienAlfa);
			this->tabPage19->Controls->Add(this->label54);
			this->tabPage19->Controls->Add(this->nowaAlfa);
			this->tabPage19->Controls->Add(this->identyfikacja_textBox);
			this->tabPage19->Controls->Add(this->czyscIdent);
			this->tabPage19->Controls->Add(this->identyfikuj);
			this->tabPage19->Location = System::Drawing::Point(4, 22);
			this->tabPage19->Name = L"tabPage19";
			this->tabPage19->Size = System::Drawing::Size(438, 494);
			this->tabPage19->TabIndex = 4;
			this->tabPage19->Text = L"Identyfikacja";
			this->tabPage19->UseVisualStyleBackColor = true;
			// 
			// zmienAlfa
			// 
			this->zmienAlfa->Location = System::Drawing::Point(158, 24);
			this->zmienAlfa->Name = L"zmienAlfa";
			this->zmienAlfa->Size = System::Drawing::Size(75, 23);
			this->zmienAlfa->TabIndex = 5;
			this->zmienAlfa->Text = L"Zmieñ";
			this->zmienAlfa->UseVisualStyleBackColor = true;
			this->zmienAlfa->Click += gcnew System::EventHandler(this, &Form1::zmienAlfa_Click);
			// 
			// label54
			// 
			this->label54->AutoSize = true;
			this->label54->Location = System::Drawing::Point(3, 29);
			this->label54->Name = L"label54";
			this->label54->Size = System::Drawing::Size(27, 13);
			this->label54->TabIndex = 4;
			this->label54->Text = L"alfa:";
			// 
			// nowaAlfa
			// 
			this->nowaAlfa->Location = System::Drawing::Point(50, 26);
			this->nowaAlfa->Name = L"nowaAlfa";
			this->nowaAlfa->Size = System::Drawing::Size(102, 20);
			this->nowaAlfa->TabIndex = 3;
			this->nowaAlfa->Text = L"0.99";
			// 
			// identyfikacja_textBox
			// 
			this->identyfikacja_textBox->Location = System::Drawing::Point(3, 52);
			this->identyfikacja_textBox->Multiline = true;
			this->identyfikacja_textBox->Name = L"identyfikacja_textBox";
			this->identyfikacja_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->identyfikacja_textBox->Size = System::Drawing::Size(420, 410);
			this->identyfikacja_textBox->TabIndex = 1;
			// 
			// czyscIdent
			// 
			this->czyscIdent->Location = System::Drawing::Point(348, 468);
			this->czyscIdent->Name = L"czyscIdent";
			this->czyscIdent->Size = System::Drawing::Size(75, 23);
			this->czyscIdent->TabIndex = 2;
			this->czyscIdent->Text = L"Wyczysc";
			this->czyscIdent->UseVisualStyleBackColor = true;
			this->czyscIdent->Click += gcnew System::EventHandler(this, &Form1::czyscIdent_Click);
			// 
			// identyfikuj
			// 
			this->identyfikuj->AutoSize = true;
			this->identyfikuj->Location = System::Drawing::Point(3, 3);
			this->identyfikuj->Name = L"identyfikuj";
			this->identyfikuj->Size = System::Drawing::Size(147, 17);
			this->identyfikuj->TabIndex = 0;
			this->identyfikuj->Text = L"Identyfikuj obiekt ARMAX";
			this->identyfikuj->UseVisualStyleBackColor = true;
			// 
			// tabPage18
			// 
			this->tabPage18->Controls->Add(this->nazwaPlikuGPC);
			this->tabPage18->Controls->Add(this->nazwaReg_GPC);
			this->tabPage18->Controls->Add(this->label75);
			this->tabPage18->Controls->Add(this->label76);
			this->tabPage18->Controls->Add(this->zapiszGPC);
			this->tabPage18->Controls->Add(this->tabControl5);
			this->tabPage18->Controls->Add(this->groupBox4);
			this->tabPage18->Location = System::Drawing::Point(4, 22);
			this->tabPage18->Name = L"tabPage18";
			this->tabPage18->Size = System::Drawing::Size(438, 494);
			this->tabPage18->TabIndex = 3;
			this->tabPage18->Text = L"Regulator GPC";
			this->tabPage18->UseVisualStyleBackColor = true;
			// 
			// nazwaPlikuGPC
			// 
			this->nazwaPlikuGPC->Location = System::Drawing::Point(98, 431);
			this->nazwaPlikuGPC->Name = L"nazwaPlikuGPC";
			this->nazwaPlikuGPC->Size = System::Drawing::Size(237, 20);
			this->nazwaPlikuGPC->TabIndex = 26;
			// 
			// nazwaReg_GPC
			// 
			this->nazwaReg_GPC->Location = System::Drawing::Point(98, 405);
			this->nazwaReg_GPC->Name = L"nazwaReg_GPC";
			this->nazwaReg_GPC->Size = System::Drawing::Size(237, 20);
			this->nazwaReg_GPC->TabIndex = 24;
			// 
			// label75
			// 
			this->label75->AutoSize = true;
			this->label75->Location = System::Drawing::Point(5, 434);
			this->label75->Name = L"label75";
			this->label75->Size = System::Drawing::Size(27, 13);
			this->label75->TabIndex = 25;
			this->label75->Text = L"Plik:";
			// 
			// label76
			// 
			this->label76->AutoSize = true;
			this->label76->Location = System::Drawing::Point(4, 408);
			this->label76->Name = L"label76";
			this->label76->Size = System::Drawing::Size(93, 13);
			this->label76->TabIndex = 23;
			this->label76->Text = L"Nazwa regulatora:";
			// 
			// zapiszGPC
			// 
			this->zapiszGPC->Location = System::Drawing::Point(249, 457);
			this->zapiszGPC->Name = L"zapiszGPC";
			this->zapiszGPC->Size = System::Drawing::Size(90, 23);
			this->zapiszGPC->TabIndex = 22;
			this->zapiszGPC->Text = L"Zapisz";
			this->zapiszGPC->UseVisualStyleBackColor = true;
			this->zapiszGPC->Click += gcnew System::EventHandler(this, &Form1::zapiszGPC_Click);
			// 
			// tabControl5
			// 
			this->tabControl5->Controls->Add(this->tabPage20);
			this->tabControl5->Controls->Add(this->tabPage21);
			this->tabControl5->Controls->Add(this->tabPage27);
			this->tabControl5->Location = System::Drawing::Point(6, 91);
			this->tabControl5->Name = L"tabControl5";
			this->tabControl5->SelectedIndex = 0;
			this->tabControl5->Size = System::Drawing::Size(417, 308);
			this->tabControl5->TabIndex = 21;
			// 
			// tabPage20
			// 
			this->tabPage20->Controls->Add(this->button4);
			this->tabPage20->Controls->Add(this->dBGPC);
			this->tabPage20->Controls->Add(this->label59);
			this->tabPage20->Controls->Add(this->button5);
			this->tabPage20->Controls->Add(this->dAGPC);
			this->tabPage20->Controls->Add(this->label60);
			this->tabPage20->Controls->Add(this->button2);
			this->tabPage20->Controls->Add(this->wyMinGPC);
			this->tabPage20->Controls->Add(this->label56);
			this->tabPage20->Controls->Add(this->button3);
			this->tabPage20->Controls->Add(this->wyMaxGPC);
			this->tabPage20->Controls->Add(this->label57);
			this->tabPage20->Controls->Add(this->zmienWszystkoGPC);
			this->tabPage20->Controls->Add(this->button8);
			this->tabPage20->Controls->Add(this->alfaGPC);
			this->tabPage20->Controls->Add(this->label61);
			this->tabPage20->Controls->Add(this->button9);
			this->tabPage20->Controls->Add(this->roGPC);
			this->tabPage20->Controls->Add(this->label62);
			this->tabPage20->Controls->Add(this->button10);
			this->tabPage20->Controls->Add(this->LGPC);
			this->tabPage20->Controls->Add(this->label63);
			this->tabPage20->Controls->Add(this->button11);
			this->tabPage20->Controls->Add(this->HGPC);
			this->tabPage20->Controls->Add(this->label64);
			this->tabPage20->Location = System::Drawing::Point(4, 22);
			this->tabPage20->Name = L"tabPage20";
			this->tabPage20->Padding = System::Windows::Forms::Padding(3);
			this->tabPage20->Size = System::Drawing::Size(409, 282);
			this->tabPage20->TabIndex = 0;
			this->tabPage20->Text = L"Zmiana parametrów";
			this->tabPage20->UseVisualStyleBackColor = true;
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(274, 133);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(46, 23);
			this->button4->TabIndex = 49;
			this->button4->Text = L"Zmieñ";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::zmiendBGPC_Click);
			// 
			// dBGPC
			// 
			this->dBGPC->Location = System::Drawing::Point(102, 135);
			this->dBGPC->Name = L"dBGPC";
			this->dBGPC->Size = System::Drawing::Size(166, 20);
			this->dBGPC->TabIndex = 48;
			// 
			// label59
			// 
			this->label59->AutoSize = true;
			this->label59->Location = System::Drawing::Point(4, 138);
			this->label59->Name = L"label59";
			this->label59->Size = System::Drawing::Size(20, 13);
			this->label59->TabIndex = 47;
			this->label59->Text = L"dB";
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(274, 107);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(46, 23);
			this->button5->TabIndex = 46;
			this->button5->Text = L"Zmieñ";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::zmiendAGPC_Click);
			// 
			// dAGPC
			// 
			this->dAGPC->Location = System::Drawing::Point(102, 109);
			this->dAGPC->Name = L"dAGPC";
			this->dAGPC->Size = System::Drawing::Size(166, 20);
			this->dAGPC->TabIndex = 45;
			// 
			// label60
			// 
			this->label60->AutoSize = true;
			this->label60->Location = System::Drawing::Point(4, 112);
			this->label60->Name = L"label60";
			this->label60->Size = System::Drawing::Size(20, 13);
			this->label60->TabIndex = 44;
			this->label60->Text = L"dA";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(273, 184);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(46, 23);
			this->button2->TabIndex = 43;
			this->button2->Text = L"Zmieñ";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::zmienWyMinGPC_Click);
			// 
			// wyMinGPC
			// 
			this->wyMinGPC->Location = System::Drawing::Point(101, 186);
			this->wyMinGPC->Name = L"wyMinGPC";
			this->wyMinGPC->Size = System::Drawing::Size(166, 20);
			this->wyMinGPC->TabIndex = 42;
			// 
			// label56
			// 
			this->label56->AutoSize = true;
			this->label56->Location = System::Drawing::Point(3, 189);
			this->label56->Name = L"label56";
			this->label56->Size = System::Drawing::Size(49, 13);
			this->label56->TabIndex = 41;
			this->label56->Text = L"Wy_Min:";
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(273, 158);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(46, 23);
			this->button3->TabIndex = 40;
			this->button3->Text = L"Zmieñ";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::zmienWyMaxGPC_Click);
			// 
			// wyMaxGPC
			// 
			this->wyMaxGPC->Location = System::Drawing::Point(101, 160);
			this->wyMaxGPC->Name = L"wyMaxGPC";
			this->wyMaxGPC->Size = System::Drawing::Size(166, 20);
			this->wyMaxGPC->TabIndex = 39;
			this->wyMaxGPC->Click += gcnew System::EventHandler(this, &Form1::zmienWyMaxGPC_Click);
			// 
			// label57
			// 
			this->label57->AutoSize = true;
			this->label57->Location = System::Drawing::Point(3, 163);
			this->label57->Name = L"label57";
			this->label57->Size = System::Drawing::Size(52, 13);
			this->label57->TabIndex = 38;
			this->label57->Text = L"Wy_Max:";
			// 
			// zmienWszystkoGPC
			// 
			this->zmienWszystkoGPC->Location = System::Drawing::Point(196, 253);
			this->zmienWszystkoGPC->Name = L"zmienWszystkoGPC";
			this->zmienWszystkoGPC->Size = System::Drawing::Size(123, 23);
			this->zmienWszystkoGPC->TabIndex = 37;
			this->zmienWszystkoGPC->Text = L"Zmieñ wszystko";
			this->zmienWszystkoGPC->UseVisualStyleBackColor = true;
			this->zmienWszystkoGPC->Click += gcnew System::EventHandler(this, &Form1::zmienWszystkoGPC_Click);
			// 
			// button8
			// 
			this->button8->Location = System::Drawing::Point(273, 82);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(46, 23);
			this->button8->TabIndex = 16;
			this->button8->Text = L"Zmieñ";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &Form1::zmienalfaGPC_Click);
			// 
			// alfaGPC
			// 
			this->alfaGPC->Location = System::Drawing::Point(101, 84);
			this->alfaGPC->Name = L"alfaGPC";
			this->alfaGPC->Size = System::Drawing::Size(166, 20);
			this->alfaGPC->TabIndex = 15;
			// 
			// label61
			// 
			this->label61->AutoSize = true;
			this->label61->Location = System::Drawing::Point(3, 87);
			this->label61->Name = L"label61";
			this->label61->Size = System::Drawing::Size(27, 13);
			this->label61->TabIndex = 14;
			this->label61->Text = L"alfa:";
			// 
			// button9
			// 
			this->button9->Location = System::Drawing::Point(273, 56);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(46, 23);
			this->button9->TabIndex = 13;
			this->button9->Text = L"Zmieñ";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &Form1::zmienroGPC_Click);
			// 
			// roGPC
			// 
			this->roGPC->Location = System::Drawing::Point(101, 58);
			this->roGPC->Name = L"roGPC";
			this->roGPC->Size = System::Drawing::Size(166, 20);
			this->roGPC->TabIndex = 12;
			// 
			// label62
			// 
			this->label62->AutoSize = true;
			this->label62->Location = System::Drawing::Point(3, 61);
			this->label62->Name = L"label62";
			this->label62->Size = System::Drawing::Size(16, 13);
			this->label62->TabIndex = 11;
			this->label62->Text = L"?:";
			// 
			// button10
			// 
			this->button10->Location = System::Drawing::Point(273, 30);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(46, 23);
			this->button10->TabIndex = 10;
			this->button10->Text = L"Zmieñ";
			this->button10->UseVisualStyleBackColor = true;
			this->button10->Click += gcnew System::EventHandler(this, &Form1::zmienLGPC_Click);
			// 
			// LGPC
			// 
			this->LGPC->Location = System::Drawing::Point(101, 32);
			this->LGPC->Name = L"LGPC";
			this->LGPC->Size = System::Drawing::Size(166, 20);
			this->LGPC->TabIndex = 9;
			// 
			// label63
			// 
			this->label63->AutoSize = true;
			this->label63->Location = System::Drawing::Point(3, 35);
			this->label63->Name = L"label63";
			this->label63->Size = System::Drawing::Size(16, 13);
			this->label63->TabIndex = 8;
			this->label63->Text = L"L:";
			// 
			// button11
			// 
			this->button11->Location = System::Drawing::Point(273, 4);
			this->button11->Name = L"button11";
			this->button11->Size = System::Drawing::Size(46, 23);
			this->button11->TabIndex = 7;
			this->button11->Text = L"Zmieñ";
			this->button11->UseVisualStyleBackColor = true;
			this->button11->Click += gcnew System::EventHandler(this, &Form1::zmienHGPC_Click);
			// 
			// HGPC
			// 
			this->HGPC->Location = System::Drawing::Point(101, 6);
			this->HGPC->Name = L"HGPC";
			this->HGPC->Size = System::Drawing::Size(166, 20);
			this->HGPC->TabIndex = 6;
			// 
			// label64
			// 
			this->label64->AutoSize = true;
			this->label64->Location = System::Drawing::Point(3, 9);
			this->label64->Name = L"label64";
			this->label64->Size = System::Drawing::Size(18, 13);
			this->label64->TabIndex = 5;
			this->label64->Text = L"H:";
			// 
			// tabPage21
			// 
			this->tabPage21->Controls->Add(this->zmienWymuszenieGPC);
			this->tabPage21->Controls->Add(this->groupBox8);
			this->tabPage21->Controls->Add(this->tabControl6);
			this->tabPage21->Location = System::Drawing::Point(4, 22);
			this->tabPage21->Name = L"tabPage21";
			this->tabPage21->Padding = System::Windows::Forms::Padding(3);
			this->tabPage21->Size = System::Drawing::Size(409, 282);
			this->tabPage21->TabIndex = 1;
			this->tabPage21->Text = L"Zmiana wymuszenia";
			this->tabPage21->UseVisualStyleBackColor = true;
			// 
			// zmienWymuszenieGPC
			// 
			this->zmienWymuszenieGPC->Location = System::Drawing::Point(244, 253);
			this->zmienWymuszenieGPC->Name = L"zmienWymuszenieGPC";
			this->zmienWymuszenieGPC->Size = System::Drawing::Size(75, 23);
			this->zmienWymuszenieGPC->TabIndex = 9;
			this->zmienWymuszenieGPC->Text = L"Zmieñ";
			this->zmienWymuszenieGPC->UseVisualStyleBackColor = true;
			this->zmienWymuszenieGPC->Click += gcnew System::EventHandler(this, &Form1::zmienWymuszenieGPC_Click);
			// 
			// groupBox8
			// 
			this->groupBox8->Controls->Add(this->skokGPC);
			this->groupBox8->Controls->Add(this->szumGPC);
			this->groupBox8->Controls->Add(this->kronekerGPC);
			this->groupBox8->Controls->Add(this->trojkatGPC);
			this->groupBox8->Controls->Add(this->prostokatGPC);
			this->groupBox8->Controls->Add(this->sinusGPC);
			this->groupBox8->ForeColor = System::Drawing::Color::Black;
			this->groupBox8->Location = System::Drawing::Point(3, 8);
			this->groupBox8->Name = L"groupBox8";
			this->groupBox8->Size = System::Drawing::Size(316, 89);
			this->groupBox8->TabIndex = 7;
			this->groupBox8->TabStop = false;
			this->groupBox8->Text = L"Sk³adowe wymuszenia";
			// 
			// skokGPC
			// 
			this->skokGPC->AutoSize = true;
			this->skokGPC->Location = System::Drawing::Point(6, 19);
			this->skokGPC->Name = L"skokGPC";
			this->skokGPC->Size = System::Drawing::Size(51, 17);
			this->skokGPC->TabIndex = 1;
			this->skokGPC->Text = L"Skok";
			this->skokGPC->UseVisualStyleBackColor = true;
			// 
			// szumGPC
			// 
			this->szumGPC->AutoSize = true;
			this->szumGPC->Location = System::Drawing::Point(128, 65);
			this->szumGPC->Name = L"szumGPC";
			this->szumGPC->Size = System::Drawing::Size(52, 17);
			this->szumGPC->TabIndex = 6;
			this->szumGPC->Text = L"Szum";
			this->szumGPC->UseVisualStyleBackColor = true;
			// 
			// kronekerGPC
			// 
			this->kronekerGPC->AutoSize = true;
			this->kronekerGPC->Location = System::Drawing::Point(6, 42);
			this->kronekerGPC->Name = L"kronekerGPC";
			this->kronekerGPC->Size = System::Drawing::Size(108, 17);
			this->kronekerGPC->TabIndex = 2;
			this->kronekerGPC->Text = L"Impuls Kronekera";
			this->kronekerGPC->UseVisualStyleBackColor = true;
			// 
			// trojkatGPC
			// 
			this->trojkatGPC->AutoSize = true;
			this->trojkatGPC->Location = System::Drawing::Point(128, 42);
			this->trojkatGPC->Name = L"trojkatGPC";
			this->trojkatGPC->Size = System::Drawing::Size(59, 17);
			this->trojkatGPC->TabIndex = 5;
			this->trojkatGPC->Text = L"Trójk¹t";
			this->trojkatGPC->UseVisualStyleBackColor = true;
			// 
			// prostokatGPC
			// 
			this->prostokatGPC->AutoSize = true;
			this->prostokatGPC->Location = System::Drawing::Point(6, 65);
			this->prostokatGPC->Name = L"prostokatGPC";
			this->prostokatGPC->Size = System::Drawing::Size(71, 17);
			this->prostokatGPC->TabIndex = 3;
			this->prostokatGPC->Text = L"Prostok¹t";
			this->prostokatGPC->UseVisualStyleBackColor = true;
			// 
			// sinusGPC
			// 
			this->sinusGPC->AutoSize = true;
			this->sinusGPC->Location = System::Drawing::Point(128, 19);
			this->sinusGPC->Name = L"sinusGPC";
			this->sinusGPC->Size = System::Drawing::Size(52, 17);
			this->sinusGPC->TabIndex = 4;
			this->sinusGPC->Text = L"Sinus";
			this->sinusGPC->UseVisualStyleBackColor = true;
			// 
			// tabControl6
			// 
			this->tabControl6->Controls->Add(this->tabPage22);
			this->tabControl6->Controls->Add(this->tabPage23);
			this->tabControl6->Controls->Add(this->tabPage24);
			this->tabControl6->Controls->Add(this->tabPage25);
			this->tabControl6->Controls->Add(this->tabPage26);
			this->tabControl6->Location = System::Drawing::Point(2, 103);
			this->tabControl6->Multiline = true;
			this->tabControl6->Name = L"tabControl6";
			this->tabControl6->SelectedIndex = 0;
			this->tabControl6->Size = System::Drawing::Size(316, 112);
			this->tabControl6->TabIndex = 8;
			// 
			// tabPage22
			// 
			this->tabPage22->Controls->Add(this->nowySkokGPC);
			this->tabPage22->Controls->Add(this->label65);
			this->tabPage22->Location = System::Drawing::Point(4, 22);
			this->tabPage22->Name = L"tabPage22";
			this->tabPage22->Padding = System::Windows::Forms::Padding(3);
			this->tabPage22->Size = System::Drawing::Size(308, 86);
			this->tabPage22->TabIndex = 0;
			this->tabPage22->Text = L"Skok";
			this->tabPage22->UseVisualStyleBackColor = true;
			// 
			// nowySkokGPC
			// 
			this->nowySkokGPC->Location = System::Drawing::Point(91, 6);
			this->nowySkokGPC->Name = L"nowySkokGPC";
			this->nowySkokGPC->Size = System::Drawing::Size(99, 20);
			this->nowySkokGPC->TabIndex = 1;
			// 
			// label65
			// 
			this->label65->AutoSize = true;
			this->label65->Location = System::Drawing::Point(6, 9);
			this->label65->Name = L"label65";
			this->label65->Size = System::Drawing::Size(79, 13);
			this->label65->TabIndex = 0;
			this->label65->Text = L"wartoœæ skoku:";
			// 
			// tabPage23
			// 
			this->tabPage23->Controls->Add(this->noweWypelnieniePGPC);
			this->tabPage23->Controls->Add(this->label66);
			this->tabPage23->Controls->Add(this->nowyOkresPGPC);
			this->tabPage23->Controls->Add(this->label67);
			this->tabPage23->Controls->Add(this->nowaAmplitudaPGPC);
			this->tabPage23->Controls->Add(this->label68);
			this->tabPage23->Location = System::Drawing::Point(4, 22);
			this->tabPage23->Name = L"tabPage23";
			this->tabPage23->Padding = System::Windows::Forms::Padding(3);
			this->tabPage23->Size = System::Drawing::Size(308, 86);
			this->tabPage23->TabIndex = 1;
			this->tabPage23->Text = L"Prostok¹t";
			this->tabPage23->UseVisualStyleBackColor = true;
			// 
			// noweWypelnieniePGPC
			// 
			this->noweWypelnieniePGPC->Location = System::Drawing::Point(79, 58);
			this->noweWypelnieniePGPC->Name = L"noweWypelnieniePGPC";
			this->noweWypelnieniePGPC->Size = System::Drawing::Size(111, 20);
			this->noweWypelnieniePGPC->TabIndex = 7;
			// 
			// label66
			// 
			this->label66->AutoSize = true;
			this->label66->Location = System::Drawing::Point(6, 61);
			this->label66->Name = L"label66";
			this->label66->Size = System::Drawing::Size(67, 13);
			this->label66->TabIndex = 6;
			this->label66->Text = L"wype³nienie:";
			// 
			// nowyOkresPGPC
			// 
			this->nowyOkresPGPC->Location = System::Drawing::Point(79, 32);
			this->nowyOkresPGPC->Name = L"nowyOkresPGPC";
			this->nowyOkresPGPC->Size = System::Drawing::Size(111, 20);
			this->nowyOkresPGPC->TabIndex = 5;
			// 
			// label67
			// 
			this->label67->AutoSize = true;
			this->label67->Location = System::Drawing::Point(6, 35);
			this->label67->Name = L"label67";
			this->label67->Size = System::Drawing::Size(36, 13);
			this->label67->TabIndex = 4;
			this->label67->Text = L"okres:";
			// 
			// nowaAmplitudaPGPC
			// 
			this->nowaAmplitudaPGPC->Location = System::Drawing::Point(79, 6);
			this->nowaAmplitudaPGPC->Name = L"nowaAmplitudaPGPC";
			this->nowaAmplitudaPGPC->Size = System::Drawing::Size(111, 20);
			this->nowaAmplitudaPGPC->TabIndex = 3;
			// 
			// label68
			// 
			this->label68->AutoSize = true;
			this->label68->Location = System::Drawing::Point(6, 9);
			this->label68->Name = L"label68";
			this->label68->Size = System::Drawing::Size(55, 13);
			this->label68->TabIndex = 2;
			this->label68->Text = L"amplituda:";
			// 
			// tabPage24
			// 
			this->tabPage24->Controls->Add(this->nowyOkresSGPC);
			this->tabPage24->Controls->Add(this->label69);
			this->tabPage24->Controls->Add(this->nowaAmplitudaSGPC);
			this->tabPage24->Controls->Add(this->label70);
			this->tabPage24->Location = System::Drawing::Point(4, 22);
			this->tabPage24->Name = L"tabPage24";
			this->tabPage24->Size = System::Drawing::Size(308, 86);
			this->tabPage24->TabIndex = 2;
			this->tabPage24->Text = L"Sinus";
			this->tabPage24->UseVisualStyleBackColor = true;
			// 
			// nowyOkresSGPC
			// 
			this->nowyOkresSGPC->Location = System::Drawing::Point(67, 32);
			this->nowyOkresSGPC->Name = L"nowyOkresSGPC";
			this->nowyOkresSGPC->Size = System::Drawing::Size(123, 20);
			this->nowyOkresSGPC->TabIndex = 9;
			// 
			// label69
			// 
			this->label69->AutoSize = true;
			this->label69->Location = System::Drawing::Point(6, 35);
			this->label69->Name = L"label69";
			this->label69->Size = System::Drawing::Size(36, 13);
			this->label69->TabIndex = 8;
			this->label69->Text = L"okres:";
			// 
			// nowaAmplitudaSGPC
			// 
			this->nowaAmplitudaSGPC->Location = System::Drawing::Point(67, 6);
			this->nowaAmplitudaSGPC->Name = L"nowaAmplitudaSGPC";
			this->nowaAmplitudaSGPC->Size = System::Drawing::Size(123, 20);
			this->nowaAmplitudaSGPC->TabIndex = 7;
			// 
			// label70
			// 
			this->label70->AutoSize = true;
			this->label70->Location = System::Drawing::Point(6, 9);
			this->label70->Name = L"label70";
			this->label70->Size = System::Drawing::Size(55, 13);
			this->label70->TabIndex = 6;
			this->label70->Text = L"amplituda:";
			// 
			// tabPage25
			// 
			this->tabPage25->Controls->Add(this->nowyOkresTGPC);
			this->tabPage25->Controls->Add(this->label71);
			this->tabPage25->Controls->Add(this->nowaAmplitudaTGPC);
			this->tabPage25->Controls->Add(this->label72);
			this->tabPage25->Location = System::Drawing::Point(4, 22);
			this->tabPage25->Name = L"tabPage25";
			this->tabPage25->Size = System::Drawing::Size(308, 86);
			this->tabPage25->TabIndex = 3;
			this->tabPage25->Text = L"Trójk¹t";
			this->tabPage25->UseVisualStyleBackColor = true;
			// 
			// nowyOkresTGPC
			// 
			this->nowyOkresTGPC->Location = System::Drawing::Point(67, 32);
			this->nowyOkresTGPC->Name = L"nowyOkresTGPC";
			this->nowyOkresTGPC->Size = System::Drawing::Size(123, 20);
			this->nowyOkresTGPC->TabIndex = 13;
			// 
			// label71
			// 
			this->label71->AutoSize = true;
			this->label71->Location = System::Drawing::Point(6, 35);
			this->label71->Name = L"label71";
			this->label71->Size = System::Drawing::Size(36, 13);
			this->label71->TabIndex = 12;
			this->label71->Text = L"okres:";
			// 
			// nowaAmplitudaTGPC
			// 
			this->nowaAmplitudaTGPC->Location = System::Drawing::Point(67, 7);
			this->nowaAmplitudaTGPC->Name = L"nowaAmplitudaTGPC";
			this->nowaAmplitudaTGPC->Size = System::Drawing::Size(123, 20);
			this->nowaAmplitudaTGPC->TabIndex = 11;
			// 
			// label72
			// 
			this->label72->AutoSize = true;
			this->label72->Location = System::Drawing::Point(6, 9);
			this->label72->Name = L"label72";
			this->label72->Size = System::Drawing::Size(55, 13);
			this->label72->TabIndex = 10;
			this->label72->Text = L"amplituda:";
			// 
			// tabPage26
			// 
			this->tabPage26->Controls->Add(this->nowaLiczbaProbekGPC);
			this->tabPage26->Controls->Add(this->label73);
			this->tabPage26->Controls->Add(this->nowaWariancjaGPC);
			this->tabPage26->Controls->Add(this->label74);
			this->tabPage26->Location = System::Drawing::Point(4, 22);
			this->tabPage26->Name = L"tabPage26";
			this->tabPage26->Size = System::Drawing::Size(308, 86);
			this->tabPage26->TabIndex = 4;
			this->tabPage26->Text = L"Szum";
			this->tabPage26->UseVisualStyleBackColor = true;
			// 
			// nowaLiczbaProbekGPC
			// 
			this->nowaLiczbaProbekGPC->Location = System::Drawing::Point(82, 32);
			this->nowaLiczbaProbekGPC->Name = L"nowaLiczbaProbekGPC";
			this->nowaLiczbaProbekGPC->Size = System::Drawing::Size(108, 20);
			this->nowaLiczbaProbekGPC->TabIndex = 13;
			// 
			// label73
			// 
			this->label73->AutoSize = true;
			this->label73->Location = System::Drawing::Point(6, 35);
			this->label73->Name = L"label73";
			this->label73->Size = System::Drawing::Size(73, 13);
			this->label73->TabIndex = 12;
			this->label73->Text = L"liczba próbek:";
			// 
			// nowaWariancjaGPC
			// 
			this->nowaWariancjaGPC->Location = System::Drawing::Point(82, 7);
			this->nowaWariancjaGPC->Name = L"nowaWariancjaGPC";
			this->nowaWariancjaGPC->Size = System::Drawing::Size(108, 20);
			this->nowaWariancjaGPC->TabIndex = 11;
			// 
			// label74
			// 
			this->label74->AutoSize = true;
			this->label74->Location = System::Drawing::Point(6, 10);
			this->label74->Name = L"label74";
			this->label74->Size = System::Drawing::Size(55, 13);
			this->label74->TabIndex = 10;
			this->label74->Text = L"wariancja:";
			// 
			// tabPage27
			// 
			this->tabPage27->Controls->Add(this->czyscIdentGPC);
			this->tabPage27->Controls->Add(this->zmiennAlfaGPC);
			this->tabPage27->Controls->Add(this->label58);
			this->tabPage27->Controls->Add(this->identyfikacjaGPC_textBox);
			this->tabPage27->Controls->Add(this->nowaAlfaGPC);
			this->tabPage27->Location = System::Drawing::Point(4, 22);
			this->tabPage27->Name = L"tabPage27";
			this->tabPage27->Size = System::Drawing::Size(409, 282);
			this->tabPage27->TabIndex = 2;
			this->tabPage27->Text = L"Identyfikacja";
			this->tabPage27->UseVisualStyleBackColor = true;
			// 
			// czyscIdentGPC
			// 
			this->czyscIdentGPC->Location = System::Drawing::Point(247, 255);
			this->czyscIdentGPC->Name = L"czyscIdentGPC";
			this->czyscIdentGPC->Size = System::Drawing::Size(75, 23);
			this->czyscIdentGPC->TabIndex = 10;
			this->czyscIdentGPC->Text = L"Wyczysc";
			this->czyscIdentGPC->UseVisualStyleBackColor = true;
			this->czyscIdentGPC->Click += gcnew System::EventHandler(this, &Form1::czyscIdentGPC_Click);
			// 
			// zmiennAlfaGPC
			// 
			this->zmiennAlfaGPC->Location = System::Drawing::Point(166, 5);
			this->zmiennAlfaGPC->Name = L"zmiennAlfaGPC";
			this->zmiennAlfaGPC->Size = System::Drawing::Size(75, 23);
			this->zmiennAlfaGPC->TabIndex = 9;
			this->zmiennAlfaGPC->Text = L"Zmieñ";
			this->zmiennAlfaGPC->UseVisualStyleBackColor = true;
			this->zmiennAlfaGPC->Click += gcnew System::EventHandler(this, &Form1::zmiennAlfaGPC_Click);
			// 
			// label58
			// 
			this->label58->AutoSize = true;
			this->label58->Location = System::Drawing::Point(11, 10);
			this->label58->Name = L"label58";
			this->label58->Size = System::Drawing::Size(27, 13);
			this->label58->TabIndex = 8;
			this->label58->Text = L"alfa:";
			// 
			// identyfikacjaGPC_textBox
			// 
			this->identyfikacjaGPC_textBox->Location = System::Drawing::Point(3, 29);
			this->identyfikacjaGPC_textBox->Multiline = true;
			this->identyfikacjaGPC_textBox->Name = L"identyfikacjaGPC_textBox";
			this->identyfikacjaGPC_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->identyfikacjaGPC_textBox->Size = System::Drawing::Size(319, 220);
			this->identyfikacjaGPC_textBox->TabIndex = 6;
			// 
			// nowaAlfaGPC
			// 
			this->nowaAlfaGPC->Location = System::Drawing::Point(58, 7);
			this->nowaAlfaGPC->Name = L"nowaAlfaGPC";
			this->nowaAlfaGPC->Size = System::Drawing::Size(102, 20);
			this->nowaAlfaGPC->TabIndex = 7;
			this->nowaAlfaGPC->Text = L"0.99";
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->wczytajRegGPC);
			this->groupBox4->Controls->Add(this->label55);
			this->groupBox4->Controls->Add(this->nazwaRegGPC);
			this->groupBox4->ForeColor = System::Drawing::Color::Black;
			this->groupBox4->Location = System::Drawing::Point(6, 6);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(417, 79);
			this->groupBox4->TabIndex = 17;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// wczytajRegGPC
			// 
			this->wczytajRegGPC->Location = System::Drawing::Point(252, 45);
			this->wczytajRegGPC->Name = L"wczytajRegGPC";
			this->wczytajRegGPC->Size = System::Drawing::Size(75, 23);
			this->wczytajRegGPC->TabIndex = 2;
			this->wczytajRegGPC->Text = L"Wczytaj";
			this->wczytajRegGPC->UseVisualStyleBackColor = true;
			this->wczytajRegGPC->Click += gcnew System::EventHandler(this, &Form1::wczytajRegGPC_Click);
			// 
			// label55
			// 
			this->label55->AutoSize = true;
			this->label55->Location = System::Drawing::Point(6, 22);
			this->label55->Name = L"label55";
			this->label55->Size = System::Drawing::Size(93, 13);
			this->label55->TabIndex = 1;
			this->label55->Text = L"Nazwa regulatora:";
			// 
			// nazwaRegGPC
			// 
			this->nazwaRegGPC->Location = System::Drawing::Point(105, 19);
			this->nazwaRegGPC->Name = L"nazwaRegGPC";
			this->nazwaRegGPC->Size = System::Drawing::Size(222, 20);
			this->nazwaRegGPC->TabIndex = 0;
			// 
			// tabPage8
			// 
			this->tabPage8->Controls->Add(this->tabControl3);
			this->tabPage8->Controls->Add(this->nazwaPlikuPID);
			this->tabPage8->Controls->Add(this->nazwaReg_PID);
			this->tabPage8->Controls->Add(this->label41);
			this->tabPage8->Controls->Add(this->groupBox7);
			this->tabPage8->Controls->Add(this->label42);
			this->tabPage8->Controls->Add(this->zapiszPID);
			this->tabPage8->Location = System::Drawing::Point(4, 22);
			this->tabPage8->Name = L"tabPage8";
			this->tabPage8->Size = System::Drawing::Size(438, 494);
			this->tabPage8->TabIndex = 2;
			this->tabPage8->Text = L"Regulator PID";
			this->tabPage8->UseVisualStyleBackColor = true;
			// 
			// tabControl3
			// 
			this->tabControl3->Controls->Add(this->tabPage14);
			this->tabControl3->Controls->Add(this->tabPage15);
			this->tabControl3->Location = System::Drawing::Point(6, 91);
			this->tabControl3->Name = L"tabControl3";
			this->tabControl3->SelectedIndex = 0;
			this->tabControl3->Size = System::Drawing::Size(417, 308);
			this->tabControl3->TabIndex = 20;
			// 
			// tabPage14
			// 
			this->tabPage14->Controls->Add(this->zmienWyMinPID);
			this->tabPage14->Controls->Add(this->wyMinPID);
			this->tabPage14->Controls->Add(this->label50);
			this->tabPage14->Controls->Add(this->zmienWyMaxPID);
			this->tabPage14->Controls->Add(this->wyMaxPID);
			this->tabPage14->Controls->Add(this->label51);
			this->tabPage14->Controls->Add(this->zmienWszystkoPID);
			this->tabPage14->Controls->Add(this->zmienBPID);
			this->tabPage14->Controls->Add(this->bPID);
			this->tabPage14->Controls->Add(this->label49);
			this->tabPage14->Controls->Add(this->zmienNPID);
			this->tabPage14->Controls->Add(this->nPID);
			this->tabPage14->Controls->Add(this->label48);
			this->tabPage14->Controls->Add(this->zmienWpPID);
			this->tabPage14->Controls->Add(this->wpPID);
			this->tabPage14->Controls->Add(this->label47);
			this->tabPage14->Controls->Add(this->zmienTpPID);
			this->tabPage14->Controls->Add(this->tpPID);
			this->tabPage14->Controls->Add(this->label46);
			this->tabPage14->Controls->Add(this->zmienTdPID);
			this->tabPage14->Controls->Add(this->tdPID);
			this->tabPage14->Controls->Add(this->label45);
			this->tabPage14->Controls->Add(this->zmienTiPID);
			this->tabPage14->Controls->Add(this->tiPID);
			this->tabPage14->Controls->Add(this->label44);
			this->tabPage14->Controls->Add(this->zmienKPID);
			this->tabPage14->Controls->Add(this->kPID);
			this->tabPage14->Controls->Add(this->label43);
			this->tabPage14->Location = System::Drawing::Point(4, 22);
			this->tabPage14->Name = L"tabPage14";
			this->tabPage14->Padding = System::Windows::Forms::Padding(3);
			this->tabPage14->Size = System::Drawing::Size(409, 282);
			this->tabPage14->TabIndex = 0;
			this->tabPage14->Text = L"Zmiana parametrów";
			this->tabPage14->UseVisualStyleBackColor = true;
			// 
			// zmienWyMinPID
			// 
			this->zmienWyMinPID->Location = System::Drawing::Point(273, 212);
			this->zmienWyMinPID->Name = L"zmienWyMinPID";
			this->zmienWyMinPID->Size = System::Drawing::Size(46, 23);
			this->zmienWyMinPID->TabIndex = 43;
			this->zmienWyMinPID->Text = L"Zmieñ";
			this->zmienWyMinPID->UseVisualStyleBackColor = true;
			this->zmienWyMinPID->Click += gcnew System::EventHandler(this, &Form1::zmienWyMinPID_Click);
			// 
			// wyMinPID
			// 
			this->wyMinPID->Location = System::Drawing::Point(101, 214);
			this->wyMinPID->Name = L"wyMinPID";
			this->wyMinPID->Size = System::Drawing::Size(166, 20);
			this->wyMinPID->TabIndex = 42;
			// 
			// label50
			// 
			this->label50->AutoSize = true;
			this->label50->Location = System::Drawing::Point(3, 217);
			this->label50->Name = L"label50";
			this->label50->Size = System::Drawing::Size(49, 13);
			this->label50->TabIndex = 41;
			this->label50->Text = L"Wy_Min:";
			// 
			// zmienWyMaxPID
			// 
			this->zmienWyMaxPID->Location = System::Drawing::Point(273, 186);
			this->zmienWyMaxPID->Name = L"zmienWyMaxPID";
			this->zmienWyMaxPID->Size = System::Drawing::Size(46, 23);
			this->zmienWyMaxPID->TabIndex = 40;
			this->zmienWyMaxPID->Text = L"Zmieñ";
			this->zmienWyMaxPID->UseVisualStyleBackColor = true;
			this->zmienWyMaxPID->Click += gcnew System::EventHandler(this, &Form1::zmienWyMaxPID_Click);
			// 
			// wyMaxPID
			// 
			this->wyMaxPID->Location = System::Drawing::Point(101, 188);
			this->wyMaxPID->Name = L"wyMaxPID";
			this->wyMaxPID->Size = System::Drawing::Size(166, 20);
			this->wyMaxPID->TabIndex = 39;
			// 
			// label51
			// 
			this->label51->AutoSize = true;
			this->label51->Location = System::Drawing::Point(3, 191);
			this->label51->Name = L"label51";
			this->label51->Size = System::Drawing::Size(52, 13);
			this->label51->TabIndex = 38;
			this->label51->Text = L"Wy_Max:";
			// 
			// zmienWszystkoPID
			// 
			this->zmienWszystkoPID->Location = System::Drawing::Point(196, 253);
			this->zmienWszystkoPID->Name = L"zmienWszystkoPID";
			this->zmienWszystkoPID->Size = System::Drawing::Size(123, 23);
			this->zmienWszystkoPID->TabIndex = 37;
			this->zmienWszystkoPID->Text = L"Zmieñ wszystko";
			this->zmienWszystkoPID->UseVisualStyleBackColor = true;
			this->zmienWszystkoPID->Click += gcnew System::EventHandler(this, &Form1::zmienWszystkoPID_Click);
			// 
			// zmienBPID
			// 
			this->zmienBPID->Location = System::Drawing::Point(273, 160);
			this->zmienBPID->Name = L"zmienBPID";
			this->zmienBPID->Size = System::Drawing::Size(46, 23);
			this->zmienBPID->TabIndex = 25;
			this->zmienBPID->Text = L"Zmieñ";
			this->zmienBPID->UseVisualStyleBackColor = true;
			this->zmienBPID->Click += gcnew System::EventHandler(this, &Form1::zmienBPID_Click);
			// 
			// bPID
			// 
			this->bPID->Location = System::Drawing::Point(101, 162);
			this->bPID->Name = L"bPID";
			this->bPID->Size = System::Drawing::Size(166, 20);
			this->bPID->TabIndex = 24;
			// 
			// label49
			// 
			this->label49->AutoSize = true;
			this->label49->Location = System::Drawing::Point(3, 165);
			this->label49->Name = L"label49";
			this->label49->Size = System::Drawing::Size(16, 13);
			this->label49->TabIndex = 23;
			this->label49->Text = L"b:";
			// 
			// zmienNPID
			// 
			this->zmienNPID->Location = System::Drawing::Point(273, 134);
			this->zmienNPID->Name = L"zmienNPID";
			this->zmienNPID->Size = System::Drawing::Size(46, 23);
			this->zmienNPID->TabIndex = 22;
			this->zmienNPID->Text = L"Zmieñ";
			this->zmienNPID->UseVisualStyleBackColor = true;
			this->zmienNPID->Click += gcnew System::EventHandler(this, &Form1::zmienNPID_Click);
			// 
			// nPID
			// 
			this->nPID->Location = System::Drawing::Point(101, 136);
			this->nPID->Name = L"nPID";
			this->nPID->Size = System::Drawing::Size(166, 20);
			this->nPID->TabIndex = 21;
			// 
			// label48
			// 
			this->label48->AutoSize = true;
			this->label48->Location = System::Drawing::Point(3, 139);
			this->label48->Name = L"label48";
			this->label48->Size = System::Drawing::Size(18, 13);
			this->label48->TabIndex = 20;
			this->label48->Text = L"N:";
			// 
			// zmienWpPID
			// 
			this->zmienWpPID->Location = System::Drawing::Point(273, 108);
			this->zmienWpPID->Name = L"zmienWpPID";
			this->zmienWpPID->Size = System::Drawing::Size(46, 23);
			this->zmienWpPID->TabIndex = 19;
			this->zmienWpPID->Text = L"Zmieñ";
			this->zmienWpPID->UseVisualStyleBackColor = true;
			this->zmienWpPID->Click += gcnew System::EventHandler(this, &Form1::zmienWpPID_Click);
			// 
			// wpPID
			// 
			this->wpPID->Location = System::Drawing::Point(101, 110);
			this->wpPID->Name = L"wpPID";
			this->wpPID->Size = System::Drawing::Size(166, 20);
			this->wpPID->TabIndex = 18;
			// 
			// label47
			// 
			this->label47->AutoSize = true;
			this->label47->Location = System::Drawing::Point(3, 113);
			this->label47->Name = L"label47";
			this->label47->Size = System::Drawing::Size(87, 13);
			this->label47->TabIndex = 17;
			this->label47->Text = L"WP ca³kowania:";
			// 
			// zmienTpPID
			// 
			this->zmienTpPID->Location = System::Drawing::Point(273, 82);
			this->zmienTpPID->Name = L"zmienTpPID";
			this->zmienTpPID->Size = System::Drawing::Size(46, 23);
			this->zmienTpPID->TabIndex = 16;
			this->zmienTpPID->Text = L"Zmieñ";
			this->zmienTpPID->UseVisualStyleBackColor = true;
			this->zmienTpPID->Click += gcnew System::EventHandler(this, &Form1::zmienTpPID_Click);
			// 
			// tpPID
			// 
			this->tpPID->Location = System::Drawing::Point(101, 84);
			this->tpPID->Name = L"tpPID";
			this->tpPID->Size = System::Drawing::Size(166, 20);
			this->tpPID->TabIndex = 15;
			// 
			// label46
			// 
			this->label46->AutoSize = true;
			this->label46->Location = System::Drawing::Point(3, 87);
			this->label46->Name = L"label46";
			this->label46->Size = System::Drawing::Size(23, 13);
			this->label46->TabIndex = 14;
			this->label46->Text = L"Tp:";
			// 
			// zmienTdPID
			// 
			this->zmienTdPID->Location = System::Drawing::Point(273, 56);
			this->zmienTdPID->Name = L"zmienTdPID";
			this->zmienTdPID->Size = System::Drawing::Size(46, 23);
			this->zmienTdPID->TabIndex = 13;
			this->zmienTdPID->Text = L"Zmieñ";
			this->zmienTdPID->UseVisualStyleBackColor = true;
			this->zmienTdPID->Click += gcnew System::EventHandler(this, &Form1::zmienTdPID_Click);
			// 
			// tdPID
			// 
			this->tdPID->Location = System::Drawing::Point(101, 58);
			this->tdPID->Name = L"tdPID";
			this->tdPID->Size = System::Drawing::Size(166, 20);
			this->tdPID->TabIndex = 12;
			// 
			// label45
			// 
			this->label45->AutoSize = true;
			this->label45->Location = System::Drawing::Point(3, 61);
			this->label45->Name = L"label45";
			this->label45->Size = System::Drawing::Size(23, 13);
			this->label45->TabIndex = 11;
			this->label45->Text = L"Td:";
			// 
			// zmienTiPID
			// 
			this->zmienTiPID->Location = System::Drawing::Point(273, 30);
			this->zmienTiPID->Name = L"zmienTiPID";
			this->zmienTiPID->Size = System::Drawing::Size(46, 23);
			this->zmienTiPID->TabIndex = 10;
			this->zmienTiPID->Text = L"Zmieñ";
			this->zmienTiPID->UseVisualStyleBackColor = true;
			this->zmienTiPID->Click += gcnew System::EventHandler(this, &Form1::zmienTiPID_Click);
			// 
			// tiPID
			// 
			this->tiPID->Location = System::Drawing::Point(101, 32);
			this->tiPID->Name = L"tiPID";
			this->tiPID->Size = System::Drawing::Size(166, 20);
			this->tiPID->TabIndex = 9;
			// 
			// label44
			// 
			this->label44->AutoSize = true;
			this->label44->Location = System::Drawing::Point(3, 35);
			this->label44->Name = L"label44";
			this->label44->Size = System::Drawing::Size(19, 13);
			this->label44->TabIndex = 8;
			this->label44->Text = L"Ti:";
			// 
			// zmienKPID
			// 
			this->zmienKPID->Location = System::Drawing::Point(273, 4);
			this->zmienKPID->Name = L"zmienKPID";
			this->zmienKPID->Size = System::Drawing::Size(46, 23);
			this->zmienKPID->TabIndex = 7;
			this->zmienKPID->Text = L"Zmieñ";
			this->zmienKPID->UseVisualStyleBackColor = true;
			this->zmienKPID->Click += gcnew System::EventHandler(this, &Form1::zmienKPID_Click);
			// 
			// kPID
			// 
			this->kPID->Location = System::Drawing::Point(101, 6);
			this->kPID->Name = L"kPID";
			this->kPID->Size = System::Drawing::Size(166, 20);
			this->kPID->TabIndex = 6;
			// 
			// label43
			// 
			this->label43->AutoSize = true;
			this->label43->Location = System::Drawing::Point(3, 9);
			this->label43->Name = L"label43";
			this->label43->Size = System::Drawing::Size(16, 13);
			this->label43->TabIndex = 5;
			this->label43->Text = L"k:";
			// 
			// tabPage15
			// 
			this->tabPage15->Controls->Add(this->zmienWymuszeniePID);
			this->tabPage15->Controls->Add(this->groupBox9);
			this->tabPage15->Controls->Add(this->tabControl2);
			this->tabPage15->Location = System::Drawing::Point(4, 22);
			this->tabPage15->Name = L"tabPage15";
			this->tabPage15->Padding = System::Windows::Forms::Padding(3);
			this->tabPage15->Size = System::Drawing::Size(409, 282);
			this->tabPage15->TabIndex = 1;
			this->tabPage15->Text = L"Zmiana wymuszenia";
			this->tabPage15->UseVisualStyleBackColor = true;
			// 
			// zmienWymuszeniePID
			// 
			this->zmienWymuszeniePID->Location = System::Drawing::Point(244, 253);
			this->zmienWymuszeniePID->Name = L"zmienWymuszeniePID";
			this->zmienWymuszeniePID->Size = System::Drawing::Size(75, 23);
			this->zmienWymuszeniePID->TabIndex = 9;
			this->zmienWymuszeniePID->Text = L"Zmieñ";
			this->zmienWymuszeniePID->UseVisualStyleBackColor = true;
			this->zmienWymuszeniePID->Click += gcnew System::EventHandler(this, &Form1::zmienWymuszeniePID_Click);
			// 
			// groupBox9
			// 
			this->groupBox9->Controls->Add(this->skokPID);
			this->groupBox9->Controls->Add(this->szumPID);
			this->groupBox9->Controls->Add(this->kronekerPID);
			this->groupBox9->Controls->Add(this->trojkatPID);
			this->groupBox9->Controls->Add(this->prostokatPID);
			this->groupBox9->Controls->Add(this->sinusPID);
			this->groupBox9->ForeColor = System::Drawing::Color::Black;
			this->groupBox9->Location = System::Drawing::Point(3, 8);
			this->groupBox9->Name = L"groupBox9";
			this->groupBox9->Size = System::Drawing::Size(316, 89);
			this->groupBox9->TabIndex = 7;
			this->groupBox9->TabStop = false;
			this->groupBox9->Text = L"Sk³adowe wymuszenia";
			// 
			// skokPID
			// 
			this->skokPID->AutoSize = true;
			this->skokPID->Location = System::Drawing::Point(6, 19);
			this->skokPID->Name = L"skokPID";
			this->skokPID->Size = System::Drawing::Size(51, 17);
			this->skokPID->TabIndex = 1;
			this->skokPID->Text = L"Skok";
			this->skokPID->UseVisualStyleBackColor = true;
			// 
			// szumPID
			// 
			this->szumPID->AutoSize = true;
			this->szumPID->Location = System::Drawing::Point(128, 65);
			this->szumPID->Name = L"szumPID";
			this->szumPID->Size = System::Drawing::Size(52, 17);
			this->szumPID->TabIndex = 6;
			this->szumPID->Text = L"Szum";
			this->szumPID->UseVisualStyleBackColor = true;
			// 
			// kronekerPID
			// 
			this->kronekerPID->AutoSize = true;
			this->kronekerPID->Location = System::Drawing::Point(6, 42);
			this->kronekerPID->Name = L"kronekerPID";
			this->kronekerPID->Size = System::Drawing::Size(108, 17);
			this->kronekerPID->TabIndex = 2;
			this->kronekerPID->Text = L"Impuls Kronekera";
			this->kronekerPID->UseVisualStyleBackColor = true;
			// 
			// trojkatPID
			// 
			this->trojkatPID->AutoSize = true;
			this->trojkatPID->Location = System::Drawing::Point(128, 42);
			this->trojkatPID->Name = L"trojkatPID";
			this->trojkatPID->Size = System::Drawing::Size(59, 17);
			this->trojkatPID->TabIndex = 5;
			this->trojkatPID->Text = L"Trójk¹t";
			this->trojkatPID->UseVisualStyleBackColor = true;
			// 
			// prostokatPID
			// 
			this->prostokatPID->AutoSize = true;
			this->prostokatPID->Location = System::Drawing::Point(6, 65);
			this->prostokatPID->Name = L"prostokatPID";
			this->prostokatPID->Size = System::Drawing::Size(71, 17);
			this->prostokatPID->TabIndex = 3;
			this->prostokatPID->Text = L"Prostok¹t";
			this->prostokatPID->UseVisualStyleBackColor = true;
			// 
			// sinusPID
			// 
			this->sinusPID->AutoSize = true;
			this->sinusPID->Location = System::Drawing::Point(128, 19);
			this->sinusPID->Name = L"sinusPID";
			this->sinusPID->Size = System::Drawing::Size(52, 17);
			this->sinusPID->TabIndex = 4;
			this->sinusPID->Text = L"Sinus";
			this->sinusPID->UseVisualStyleBackColor = true;
			// 
			// tabControl2
			// 
			this->tabControl2->Controls->Add(this->tabPage9);
			this->tabControl2->Controls->Add(this->tabPage10);
			this->tabControl2->Controls->Add(this->tabPage11);
			this->tabControl2->Controls->Add(this->tabPage12);
			this->tabControl2->Controls->Add(this->tabPage13);
			this->tabControl2->Location = System::Drawing::Point(2, 103);
			this->tabControl2->Multiline = true;
			this->tabControl2->Name = L"tabControl2";
			this->tabControl2->SelectedIndex = 0;
			this->tabControl2->Size = System::Drawing::Size(316, 112);
			this->tabControl2->TabIndex = 8;
			// 
			// tabPage9
			// 
			this->tabPage9->Controls->Add(this->nowySkokPID);
			this->tabPage9->Controls->Add(this->label31);
			this->tabPage9->Location = System::Drawing::Point(4, 22);
			this->tabPage9->Name = L"tabPage9";
			this->tabPage9->Padding = System::Windows::Forms::Padding(3);
			this->tabPage9->Size = System::Drawing::Size(308, 86);
			this->tabPage9->TabIndex = 0;
			this->tabPage9->Text = L"Skok";
			this->tabPage9->UseVisualStyleBackColor = true;
			// 
			// nowySkokPID
			// 
			this->nowySkokPID->Location = System::Drawing::Point(91, 6);
			this->nowySkokPID->Name = L"nowySkokPID";
			this->nowySkokPID->Size = System::Drawing::Size(99, 20);
			this->nowySkokPID->TabIndex = 1;
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(6, 9);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(79, 13);
			this->label31->TabIndex = 0;
			this->label31->Text = L"wartoœæ skoku:";
			// 
			// tabPage10
			// 
			this->tabPage10->Controls->Add(this->noweWypelnieniePPID);
			this->tabPage10->Controls->Add(this->label32);
			this->tabPage10->Controls->Add(this->nowyOkresPPID);
			this->tabPage10->Controls->Add(this->label33);
			this->tabPage10->Controls->Add(this->nowaAmplitudaPPID);
			this->tabPage10->Controls->Add(this->label34);
			this->tabPage10->Location = System::Drawing::Point(4, 22);
			this->tabPage10->Name = L"tabPage10";
			this->tabPage10->Padding = System::Windows::Forms::Padding(3);
			this->tabPage10->Size = System::Drawing::Size(308, 86);
			this->tabPage10->TabIndex = 1;
			this->tabPage10->Text = L"Prostok¹t";
			this->tabPage10->UseVisualStyleBackColor = true;
			// 
			// noweWypelnieniePPID
			// 
			this->noweWypelnieniePPID->Location = System::Drawing::Point(79, 58);
			this->noweWypelnieniePPID->Name = L"noweWypelnieniePPID";
			this->noweWypelnieniePPID->Size = System::Drawing::Size(111, 20);
			this->noweWypelnieniePPID->TabIndex = 7;
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(6, 61);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(67, 13);
			this->label32->TabIndex = 6;
			this->label32->Text = L"wype³nienie:";
			// 
			// nowyOkresPPID
			// 
			this->nowyOkresPPID->Location = System::Drawing::Point(79, 32);
			this->nowyOkresPPID->Name = L"nowyOkresPPID";
			this->nowyOkresPPID->Size = System::Drawing::Size(111, 20);
			this->nowyOkresPPID->TabIndex = 5;
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Location = System::Drawing::Point(6, 35);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(36, 13);
			this->label33->TabIndex = 4;
			this->label33->Text = L"okres:";
			// 
			// nowaAmplitudaPPID
			// 
			this->nowaAmplitudaPPID->Location = System::Drawing::Point(79, 6);
			this->nowaAmplitudaPPID->Name = L"nowaAmplitudaPPID";
			this->nowaAmplitudaPPID->Size = System::Drawing::Size(111, 20);
			this->nowaAmplitudaPPID->TabIndex = 3;
			// 
			// label34
			// 
			this->label34->AutoSize = true;
			this->label34->Location = System::Drawing::Point(6, 9);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(55, 13);
			this->label34->TabIndex = 2;
			this->label34->Text = L"amplituda:";
			// 
			// tabPage11
			// 
			this->tabPage11->Controls->Add(this->nowyOkresSPID);
			this->tabPage11->Controls->Add(this->label35);
			this->tabPage11->Controls->Add(this->nowaAmplitudaSPID);
			this->tabPage11->Controls->Add(this->label36);
			this->tabPage11->Location = System::Drawing::Point(4, 22);
			this->tabPage11->Name = L"tabPage11";
			this->tabPage11->Size = System::Drawing::Size(308, 86);
			this->tabPage11->TabIndex = 2;
			this->tabPage11->Text = L"Sinus";
			this->tabPage11->UseVisualStyleBackColor = true;
			// 
			// nowyOkresSPID
			// 
			this->nowyOkresSPID->Location = System::Drawing::Point(67, 32);
			this->nowyOkresSPID->Name = L"nowyOkresSPID";
			this->nowyOkresSPID->Size = System::Drawing::Size(123, 20);
			this->nowyOkresSPID->TabIndex = 9;
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(6, 35);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(36, 13);
			this->label35->TabIndex = 8;
			this->label35->Text = L"okres:";
			// 
			// nowaAmplitudaSPID
			// 
			this->nowaAmplitudaSPID->Location = System::Drawing::Point(67, 6);
			this->nowaAmplitudaSPID->Name = L"nowaAmplitudaSPID";
			this->nowaAmplitudaSPID->Size = System::Drawing::Size(123, 20);
			this->nowaAmplitudaSPID->TabIndex = 7;
			// 
			// label36
			// 
			this->label36->AutoSize = true;
			this->label36->Location = System::Drawing::Point(6, 9);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(55, 13);
			this->label36->TabIndex = 6;
			this->label36->Text = L"amplituda:";
			// 
			// tabPage12
			// 
			this->tabPage12->Controls->Add(this->nowyOkresTPID);
			this->tabPage12->Controls->Add(this->label37);
			this->tabPage12->Controls->Add(this->nowaAmplitudaTPID);
			this->tabPage12->Controls->Add(this->label38);
			this->tabPage12->Location = System::Drawing::Point(4, 22);
			this->tabPage12->Name = L"tabPage12";
			this->tabPage12->Size = System::Drawing::Size(308, 86);
			this->tabPage12->TabIndex = 3;
			this->tabPage12->Text = L"Trójk¹t";
			this->tabPage12->UseVisualStyleBackColor = true;
			// 
			// nowyOkresTPID
			// 
			this->nowyOkresTPID->Location = System::Drawing::Point(67, 32);
			this->nowyOkresTPID->Name = L"nowyOkresTPID";
			this->nowyOkresTPID->Size = System::Drawing::Size(123, 20);
			this->nowyOkresTPID->TabIndex = 13;
			// 
			// label37
			// 
			this->label37->AutoSize = true;
			this->label37->Location = System::Drawing::Point(6, 35);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(36, 13);
			this->label37->TabIndex = 12;
			this->label37->Text = L"okres:";
			// 
			// nowaAmplitudaTPID
			// 
			this->nowaAmplitudaTPID->Location = System::Drawing::Point(67, 7);
			this->nowaAmplitudaTPID->Name = L"nowaAmplitudaTPID";
			this->nowaAmplitudaTPID->Size = System::Drawing::Size(123, 20);
			this->nowaAmplitudaTPID->TabIndex = 11;
			// 
			// label38
			// 
			this->label38->AutoSize = true;
			this->label38->Location = System::Drawing::Point(6, 9);
			this->label38->Name = L"label38";
			this->label38->Size = System::Drawing::Size(55, 13);
			this->label38->TabIndex = 10;
			this->label38->Text = L"amplituda:";
			// 
			// tabPage13
			// 
			this->tabPage13->Controls->Add(this->nowaLiczbaProbekPID);
			this->tabPage13->Controls->Add(this->label39);
			this->tabPage13->Controls->Add(this->nowaWariancjaPID);
			this->tabPage13->Controls->Add(this->label40);
			this->tabPage13->Location = System::Drawing::Point(4, 22);
			this->tabPage13->Name = L"tabPage13";
			this->tabPage13->Size = System::Drawing::Size(308, 86);
			this->tabPage13->TabIndex = 4;
			this->tabPage13->Text = L"Szum";
			this->tabPage13->UseVisualStyleBackColor = true;
			// 
			// nowaLiczbaProbekPID
			// 
			this->nowaLiczbaProbekPID->Location = System::Drawing::Point(82, 32);
			this->nowaLiczbaProbekPID->Name = L"nowaLiczbaProbekPID";
			this->nowaLiczbaProbekPID->Size = System::Drawing::Size(108, 20);
			this->nowaLiczbaProbekPID->TabIndex = 13;
			// 
			// label39
			// 
			this->label39->AutoSize = true;
			this->label39->Location = System::Drawing::Point(6, 35);
			this->label39->Name = L"label39";
			this->label39->Size = System::Drawing::Size(73, 13);
			this->label39->TabIndex = 12;
			this->label39->Text = L"liczba próbek:";
			// 
			// nowaWariancjaPID
			// 
			this->nowaWariancjaPID->Location = System::Drawing::Point(82, 7);
			this->nowaWariancjaPID->Name = L"nowaWariancjaPID";
			this->nowaWariancjaPID->Size = System::Drawing::Size(108, 20);
			this->nowaWariancjaPID->TabIndex = 11;
			// 
			// label40
			// 
			this->label40->AutoSize = true;
			this->label40->Location = System::Drawing::Point(6, 10);
			this->label40->Name = L"label40";
			this->label40->Size = System::Drawing::Size(55, 13);
			this->label40->TabIndex = 10;
			this->label40->Text = L"wariancja:";
			// 
			// nazwaPlikuPID
			// 
			this->nazwaPlikuPID->Location = System::Drawing::Point(98, 431);
			this->nazwaPlikuPID->Name = L"nazwaPlikuPID";
			this->nazwaPlikuPID->Size = System::Drawing::Size(237, 20);
			this->nazwaPlikuPID->TabIndex = 19;
			// 
			// nazwaReg_PID
			// 
			this->nazwaReg_PID->Location = System::Drawing::Point(98, 405);
			this->nazwaReg_PID->Name = L"nazwaReg_PID";
			this->nazwaReg_PID->Size = System::Drawing::Size(237, 20);
			this->nazwaReg_PID->TabIndex = 17;
			// 
			// label41
			// 
			this->label41->AutoSize = true;
			this->label41->Location = System::Drawing::Point(5, 434);
			this->label41->Name = L"label41";
			this->label41->Size = System::Drawing::Size(27, 13);
			this->label41->TabIndex = 18;
			this->label41->Text = L"Plik:";
			// 
			// groupBox7
			// 
			this->groupBox7->Controls->Add(this->wczytajRegPID);
			this->groupBox7->Controls->Add(this->label30);
			this->groupBox7->Controls->Add(this->nazwaRegPID);
			this->groupBox7->ForeColor = System::Drawing::Color::Black;
			this->groupBox7->Location = System::Drawing::Point(6, 6);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Size = System::Drawing::Size(417, 79);
			this->groupBox7->TabIndex = 16;
			this->groupBox7->TabStop = false;
			this->groupBox7->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// wczytajRegPID
			// 
			this->wczytajRegPID->Location = System::Drawing::Point(252, 45);
			this->wczytajRegPID->Name = L"wczytajRegPID";
			this->wczytajRegPID->Size = System::Drawing::Size(75, 23);
			this->wczytajRegPID->TabIndex = 2;
			this->wczytajRegPID->Text = L"Wczytaj";
			this->wczytajRegPID->UseVisualStyleBackColor = true;
			this->wczytajRegPID->Click += gcnew System::EventHandler(this, &Form1::wczytajRegPID_Click);
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Location = System::Drawing::Point(6, 22);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(93, 13);
			this->label30->TabIndex = 1;
			this->label30->Text = L"Nazwa regulatora:";
			// 
			// nazwaRegPID
			// 
			this->nazwaRegPID->Location = System::Drawing::Point(105, 19);
			this->nazwaRegPID->Name = L"nazwaRegPID";
			this->nazwaRegPID->Size = System::Drawing::Size(222, 20);
			this->nazwaRegPID->TabIndex = 0;
			// 
			// label42
			// 
			this->label42->AutoSize = true;
			this->label42->Location = System::Drawing::Point(4, 408);
			this->label42->Name = L"label42";
			this->label42->Size = System::Drawing::Size(93, 13);
			this->label42->TabIndex = 16;
			this->label42->Text = L"Nazwa regulatora:";
			// 
			// zapiszPID
			// 
			this->zapiszPID->Location = System::Drawing::Point(249, 457);
			this->zapiszPID->Name = L"zapiszPID";
			this->zapiszPID->Size = System::Drawing::Size(90, 23);
			this->zapiszPID->TabIndex = 15;
			this->zapiszPID->Text = L"Zapisz";
			this->zapiszPID->UseVisualStyleBackColor = true;
			this->zapiszPID->Click += gcnew System::EventHandler(this, &Form1::zapiszPID_Click);
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->tabControl4);
			this->tabPage2->Controls->Add(this->nazwaPlikuP);
			this->tabPage2->Controls->Add(this->nazwaReg_P);
			this->tabPage2->Controls->Add(this->label28);
			this->tabPage2->Controls->Add(this->groupBox5);
			this->tabPage2->Controls->Add(this->label27);
			this->tabPage2->Controls->Add(this->zapiszP);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(438, 494);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Regulator P";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// tabControl4
			// 
			this->tabControl4->Controls->Add(this->tabPage16);
			this->tabControl4->Controls->Add(this->tabPage17);
			this->tabControl4->Location = System::Drawing::Point(6, 91);
			this->tabControl4->Name = L"tabControl4";
			this->tabControl4->SelectedIndex = 0;
			this->tabControl4->Size = System::Drawing::Size(417, 308);
			this->tabControl4->TabIndex = 12;
			// 
			// tabPage16
			// 
			this->tabPage16->Controls->Add(this->zmienWszystkoP);
			this->tabPage16->Controls->Add(this->zmienWyMinP);
			this->tabPage16->Controls->Add(this->wyMinP);
			this->tabPage16->Controls->Add(this->label52);
			this->tabPage16->Controls->Add(this->zmienWyMaxP);
			this->tabPage16->Controls->Add(this->wyMaxP);
			this->tabPage16->Controls->Add(this->label53);
			this->tabPage16->Controls->Add(this->k);
			this->tabPage16->Controls->Add(this->label15);
			this->tabPage16->Controls->Add(this->zmienK);
			this->tabPage16->Location = System::Drawing::Point(4, 22);
			this->tabPage16->Name = L"tabPage16";
			this->tabPage16->Padding = System::Windows::Forms::Padding(3);
			this->tabPage16->Size = System::Drawing::Size(409, 282);
			this->tabPage16->TabIndex = 0;
			this->tabPage16->Text = L"Zmiana parametrów";
			this->tabPage16->UseVisualStyleBackColor = true;
			// 
			// zmienWszystkoP
			// 
			this->zmienWszystkoP->Location = System::Drawing::Point(196, 253);
			this->zmienWszystkoP->Name = L"zmienWszystkoP";
			this->zmienWszystkoP->Size = System::Drawing::Size(123, 23);
			this->zmienWszystkoP->TabIndex = 50;
			this->zmienWszystkoP->Text = L"Zmieñ wszystko";
			this->zmienWszystkoP->UseVisualStyleBackColor = true;
			this->zmienWszystkoP->Click += gcnew System::EventHandler(this, &Form1::zmienWszystkoP_Click);
			// 
			// zmienWyMinP
			// 
			this->zmienWyMinP->Location = System::Drawing::Point(273, 56);
			this->zmienWyMinP->Name = L"zmienWyMinP";
			this->zmienWyMinP->Size = System::Drawing::Size(46, 23);
			this->zmienWyMinP->TabIndex = 49;
			this->zmienWyMinP->Text = L"Zmieñ";
			this->zmienWyMinP->UseVisualStyleBackColor = true;
			this->zmienWyMinP->Click += gcnew System::EventHandler(this, &Form1::zmienWyMinP_Click);
			// 
			// wyMinP
			// 
			this->wyMinP->Location = System::Drawing::Point(58, 58);
			this->wyMinP->Name = L"wyMinP";
			this->wyMinP->Size = System::Drawing::Size(209, 20);
			this->wyMinP->TabIndex = 48;
			// 
			// label52
			// 
			this->label52->AutoSize = true;
			this->label52->Location = System::Drawing::Point(3, 61);
			this->label52->Name = L"label52";
			this->label52->Size = System::Drawing::Size(49, 13);
			this->label52->TabIndex = 47;
			this->label52->Text = L"Wy_Min:";
			// 
			// zmienWyMaxP
			// 
			this->zmienWyMaxP->Location = System::Drawing::Point(273, 30);
			this->zmienWyMaxP->Name = L"zmienWyMaxP";
			this->zmienWyMaxP->Size = System::Drawing::Size(46, 23);
			this->zmienWyMaxP->TabIndex = 46;
			this->zmienWyMaxP->Text = L"Zmieñ";
			this->zmienWyMaxP->UseVisualStyleBackColor = true;
			this->zmienWyMaxP->Click += gcnew System::EventHandler(this, &Form1::zmienWyMaxP_Click);
			// 
			// wyMaxP
			// 
			this->wyMaxP->Location = System::Drawing::Point(58, 32);
			this->wyMaxP->Name = L"wyMaxP";
			this->wyMaxP->Size = System::Drawing::Size(209, 20);
			this->wyMaxP->TabIndex = 45;
			// 
			// label53
			// 
			this->label53->AutoSize = true;
			this->label53->Location = System::Drawing::Point(3, 35);
			this->label53->Name = L"label53";
			this->label53->Size = System::Drawing::Size(52, 13);
			this->label53->TabIndex = 44;
			this->label53->Text = L"Wy_Max:";
			// 
			// k
			// 
			this->k->Location = System::Drawing::Point(58, 6);
			this->k->Name = L"k";
			this->k->Size = System::Drawing::Size(209, 20);
			this->k->TabIndex = 14;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(3, 9);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(16, 13);
			this->label15->TabIndex = 13;
			this->label15->Text = L"k:";
			// 
			// zmienK
			// 
			this->zmienK->Location = System::Drawing::Point(273, 4);
			this->zmienK->Name = L"zmienK";
			this->zmienK->Size = System::Drawing::Size(46, 23);
			this->zmienK->TabIndex = 12;
			this->zmienK->Text = L"Zmieñ";
			this->zmienK->UseVisualStyleBackColor = true;
			this->zmienK->Click += gcnew System::EventHandler(this, &Form1::zmienP_Click);
			// 
			// tabPage17
			// 
			this->tabPage17->Controls->Add(this->zmienWymuszenieP);
			this->tabPage17->Controls->Add(this->groupBox6);
			this->tabPage17->Controls->Add(this->tabControl1);
			this->tabPage17->Location = System::Drawing::Point(4, 22);
			this->tabPage17->Name = L"tabPage17";
			this->tabPage17->Padding = System::Windows::Forms::Padding(3);
			this->tabPage17->Size = System::Drawing::Size(409, 282);
			this->tabPage17->TabIndex = 1;
			this->tabPage17->Text = L"Zmiana wymuszenia";
			this->tabPage17->UseVisualStyleBackColor = true;
			// 
			// zmienWymuszenieP
			// 
			this->zmienWymuszenieP->Location = System::Drawing::Point(244, 253);
			this->zmienWymuszenieP->Name = L"zmienWymuszenieP";
			this->zmienWymuszenieP->Size = System::Drawing::Size(75, 23);
			this->zmienWymuszenieP->TabIndex = 9;
			this->zmienWymuszenieP->Text = L"Zmieñ";
			this->zmienWymuszenieP->UseVisualStyleBackColor = true;
			this->zmienWymuszenieP->Click += gcnew System::EventHandler(this, &Form1::zmienWymuszenieP_Click);
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->skokP);
			this->groupBox6->Controls->Add(this->szumP);
			this->groupBox6->Controls->Add(this->kronekerP);
			this->groupBox6->Controls->Add(this->trojkatP);
			this->groupBox6->Controls->Add(this->prostokatP);
			this->groupBox6->Controls->Add(this->sinusP);
			this->groupBox6->ForeColor = System::Drawing::Color::Black;
			this->groupBox6->Location = System::Drawing::Point(3, 8);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(316, 89);
			this->groupBox6->TabIndex = 7;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Sk³adowe wymuszenia";
			// 
			// skokP
			// 
			this->skokP->AutoSize = true;
			this->skokP->Location = System::Drawing::Point(6, 19);
			this->skokP->Name = L"skokP";
			this->skokP->Size = System::Drawing::Size(51, 17);
			this->skokP->TabIndex = 1;
			this->skokP->Text = L"Skok";
			this->skokP->UseVisualStyleBackColor = true;
			// 
			// szumP
			// 
			this->szumP->AutoSize = true;
			this->szumP->Location = System::Drawing::Point(128, 65);
			this->szumP->Name = L"szumP";
			this->szumP->Size = System::Drawing::Size(52, 17);
			this->szumP->TabIndex = 6;
			this->szumP->Text = L"Szum";
			this->szumP->UseVisualStyleBackColor = true;
			// 
			// kronekerP
			// 
			this->kronekerP->AutoSize = true;
			this->kronekerP->Location = System::Drawing::Point(6, 42);
			this->kronekerP->Name = L"kronekerP";
			this->kronekerP->Size = System::Drawing::Size(108, 17);
			this->kronekerP->TabIndex = 2;
			this->kronekerP->Text = L"Impuls Kronekera";
			this->kronekerP->UseVisualStyleBackColor = true;
			// 
			// trojkatP
			// 
			this->trojkatP->AutoSize = true;
			this->trojkatP->Location = System::Drawing::Point(128, 42);
			this->trojkatP->Name = L"trojkatP";
			this->trojkatP->Size = System::Drawing::Size(59, 17);
			this->trojkatP->TabIndex = 5;
			this->trojkatP->Text = L"Trójk¹t";
			this->trojkatP->UseVisualStyleBackColor = true;
			// 
			// prostokatP
			// 
			this->prostokatP->AutoSize = true;
			this->prostokatP->Location = System::Drawing::Point(6, 65);
			this->prostokatP->Name = L"prostokatP";
			this->prostokatP->Size = System::Drawing::Size(71, 17);
			this->prostokatP->TabIndex = 3;
			this->prostokatP->Text = L"Prostok¹t";
			this->prostokatP->UseVisualStyleBackColor = true;
			// 
			// sinusP
			// 
			this->sinusP->AutoSize = true;
			this->sinusP->Location = System::Drawing::Point(128, 19);
			this->sinusP->Name = L"sinusP";
			this->sinusP->Size = System::Drawing::Size(52, 17);
			this->sinusP->TabIndex = 4;
			this->sinusP->Text = L"Sinus";
			this->sinusP->UseVisualStyleBackColor = true;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Controls->Add(this->tabPage4);
			this->tabControl1->Controls->Add(this->tabPage5);
			this->tabControl1->Controls->Add(this->tabPage6);
			this->tabControl1->Controls->Add(this->tabPage7);
			this->tabControl1->Location = System::Drawing::Point(2, 103);
			this->tabControl1->Multiline = true;
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(317, 112);
			this->tabControl1->TabIndex = 8;
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->nowySkokP);
			this->tabPage3->Controls->Add(this->label17);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(309, 86);
			this->tabPage3->TabIndex = 0;
			this->tabPage3->Text = L"Skok";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// nowySkokP
			// 
			this->nowySkokP->Location = System::Drawing::Point(91, 6);
			this->nowySkokP->Name = L"nowySkokP";
			this->nowySkokP->Size = System::Drawing::Size(99, 20);
			this->nowySkokP->TabIndex = 1;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(6, 9);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(79, 13);
			this->label17->TabIndex = 0;
			this->label17->Text = L"wartoœæ skoku:";
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->noweWypelnieniePP);
			this->tabPage4->Controls->Add(this->label20);
			this->tabPage4->Controls->Add(this->nowyOkresPP);
			this->tabPage4->Controls->Add(this->label19);
			this->tabPage4->Controls->Add(this->nowaAmplitudaPP);
			this->tabPage4->Controls->Add(this->label18);
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(309, 86);
			this->tabPage4->TabIndex = 1;
			this->tabPage4->Text = L"Prostok¹t";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// noweWypelnieniePP
			// 
			this->noweWypelnieniePP->Location = System::Drawing::Point(79, 58);
			this->noweWypelnieniePP->Name = L"noweWypelnieniePP";
			this->noweWypelnieniePP->Size = System::Drawing::Size(111, 20);
			this->noweWypelnieniePP->TabIndex = 7;
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(6, 61);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(67, 13);
			this->label20->TabIndex = 6;
			this->label20->Text = L"wype³nienie:";
			// 
			// nowyOkresPP
			// 
			this->nowyOkresPP->Location = System::Drawing::Point(79, 32);
			this->nowyOkresPP->Name = L"nowyOkresPP";
			this->nowyOkresPP->Size = System::Drawing::Size(111, 20);
			this->nowyOkresPP->TabIndex = 5;
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(6, 35);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(36, 13);
			this->label19->TabIndex = 4;
			this->label19->Text = L"okres:";
			// 
			// nowaAmplitudaPP
			// 
			this->nowaAmplitudaPP->Location = System::Drawing::Point(79, 6);
			this->nowaAmplitudaPP->Name = L"nowaAmplitudaPP";
			this->nowaAmplitudaPP->Size = System::Drawing::Size(111, 20);
			this->nowaAmplitudaPP->TabIndex = 3;
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(6, 9);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(55, 13);
			this->label18->TabIndex = 2;
			this->label18->Text = L"amplituda:";
			// 
			// tabPage5
			// 
			this->tabPage5->Controls->Add(this->nowyOkresSP);
			this->tabPage5->Controls->Add(this->label21);
			this->tabPage5->Controls->Add(this->nowaAmplitudaSP);
			this->tabPage5->Controls->Add(this->label22);
			this->tabPage5->Location = System::Drawing::Point(4, 22);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Size = System::Drawing::Size(309, 86);
			this->tabPage5->TabIndex = 2;
			this->tabPage5->Text = L"Sinus";
			this->tabPage5->UseVisualStyleBackColor = true;
			// 
			// nowyOkresSP
			// 
			this->nowyOkresSP->Location = System::Drawing::Point(67, 32);
			this->nowyOkresSP->Name = L"nowyOkresSP";
			this->nowyOkresSP->Size = System::Drawing::Size(123, 20);
			this->nowyOkresSP->TabIndex = 9;
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(6, 35);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(36, 13);
			this->label21->TabIndex = 8;
			this->label21->Text = L"okres:";
			// 
			// nowaAmplitudaSP
			// 
			this->nowaAmplitudaSP->Location = System::Drawing::Point(67, 6);
			this->nowaAmplitudaSP->Name = L"nowaAmplitudaSP";
			this->nowaAmplitudaSP->Size = System::Drawing::Size(123, 20);
			this->nowaAmplitudaSP->TabIndex = 7;
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(6, 9);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(55, 13);
			this->label22->TabIndex = 6;
			this->label22->Text = L"amplituda:";
			// 
			// tabPage6
			// 
			this->tabPage6->Controls->Add(this->nowyOkresTP);
			this->tabPage6->Controls->Add(this->label23);
			this->tabPage6->Controls->Add(this->nowaAmplitudaTP);
			this->tabPage6->Controls->Add(this->label24);
			this->tabPage6->Location = System::Drawing::Point(4, 22);
			this->tabPage6->Name = L"tabPage6";
			this->tabPage6->Size = System::Drawing::Size(309, 86);
			this->tabPage6->TabIndex = 3;
			this->tabPage6->Text = L"Trójk¹t";
			this->tabPage6->UseVisualStyleBackColor = true;
			// 
			// nowyOkresTP
			// 
			this->nowyOkresTP->Location = System::Drawing::Point(67, 32);
			this->nowyOkresTP->Name = L"nowyOkresTP";
			this->nowyOkresTP->Size = System::Drawing::Size(123, 20);
			this->nowyOkresTP->TabIndex = 13;
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(6, 35);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(36, 13);
			this->label23->TabIndex = 12;
			this->label23->Text = L"okres:";
			// 
			// nowaAmplitudaTP
			// 
			this->nowaAmplitudaTP->Location = System::Drawing::Point(67, 6);
			this->nowaAmplitudaTP->Name = L"nowaAmplitudaTP";
			this->nowaAmplitudaTP->Size = System::Drawing::Size(123, 20);
			this->nowaAmplitudaTP->TabIndex = 11;
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(6, 9);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(55, 13);
			this->label24->TabIndex = 10;
			this->label24->Text = L"amplituda:";
			// 
			// tabPage7
			// 
			this->tabPage7->Controls->Add(this->nowaLiczbaProbekP);
			this->tabPage7->Controls->Add(this->label25);
			this->tabPage7->Controls->Add(this->nowaWariancjaP);
			this->tabPage7->Controls->Add(this->label26);
			this->tabPage7->Location = System::Drawing::Point(4, 22);
			this->tabPage7->Name = L"tabPage7";
			this->tabPage7->Size = System::Drawing::Size(309, 86);
			this->tabPage7->TabIndex = 4;
			this->tabPage7->Text = L"Szum";
			this->tabPage7->UseVisualStyleBackColor = true;
			// 
			// nowaLiczbaProbekP
			// 
			this->nowaLiczbaProbekP->Location = System::Drawing::Point(82, 32);
			this->nowaLiczbaProbekP->Name = L"nowaLiczbaProbekP";
			this->nowaLiczbaProbekP->Size = System::Drawing::Size(108, 20);
			this->nowaLiczbaProbekP->TabIndex = 13;
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(6, 35);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(73, 13);
			this->label25->TabIndex = 12;
			this->label25->Text = L"liczba próbek:";
			// 
			// nowaWariancjaP
			// 
			this->nowaWariancjaP->Location = System::Drawing::Point(82, 7);
			this->nowaWariancjaP->Name = L"nowaWariancjaP";
			this->nowaWariancjaP->Size = System::Drawing::Size(108, 20);
			this->nowaWariancjaP->TabIndex = 11;
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(6, 10);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(55, 13);
			this->label26->TabIndex = 10;
			this->label26->Text = L"wariancja:";
			// 
			// nazwaPlikuP
			// 
			this->nazwaPlikuP->Location = System::Drawing::Point(98, 431);
			this->nazwaPlikuP->Name = L"nazwaPlikuP";
			this->nazwaPlikuP->Size = System::Drawing::Size(237, 20);
			this->nazwaPlikuP->TabIndex = 14;
			// 
			// nazwaReg_P
			// 
			this->nazwaReg_P->Location = System::Drawing::Point(98, 405);
			this->nazwaReg_P->Name = L"nazwaReg_P";
			this->nazwaReg_P->Size = System::Drawing::Size(237, 20);
			this->nazwaReg_P->TabIndex = 12;
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(5, 434);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(27, 13);
			this->label28->TabIndex = 13;
			this->label28->Text = L"Plik:";
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->wczytajRegP);
			this->groupBox5->Controls->Add(this->label16);
			this->groupBox5->Controls->Add(this->nazwaRegP);
			this->groupBox5->ForeColor = System::Drawing::Color::Black;
			this->groupBox5->Location = System::Drawing::Point(6, 6);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(417, 79);
			this->groupBox5->TabIndex = 15;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// wczytajRegP
			// 
			this->wczytajRegP->Location = System::Drawing::Point(252, 45);
			this->wczytajRegP->Name = L"wczytajRegP";
			this->wczytajRegP->Size = System::Drawing::Size(75, 23);
			this->wczytajRegP->TabIndex = 2;
			this->wczytajRegP->Text = L"Wczytaj";
			this->wczytajRegP->UseVisualStyleBackColor = true;
			this->wczytajRegP->Click += gcnew System::EventHandler(this, &Form1::wczytajRegP_Click);
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(6, 22);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(93, 13);
			this->label16->TabIndex = 1;
			this->label16->Text = L"Nazwa regulatora:";
			// 
			// nazwaRegP
			// 
			this->nazwaRegP->Location = System::Drawing::Point(105, 19);
			this->nazwaRegP->Name = L"nazwaRegP";
			this->nazwaRegP->Size = System::Drawing::Size(222, 20);
			this->nazwaRegP->TabIndex = 0;
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(4, 408);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(93, 13);
			this->label27->TabIndex = 11;
			this->label27->Text = L"Nazwa regulatora:";
			// 
			// zapiszP
			// 
			this->zapiszP->Location = System::Drawing::Point(245, 457);
			this->zapiszP->Name = L"zapiszP";
			this->zapiszP->Size = System::Drawing::Size(90, 23);
			this->zapiszP->TabIndex = 10;
			this->zapiszP->Text = L"Zapisz";
			this->zapiszP->UseVisualStyleBackColor = true;
			this->zapiszP->Click += gcnew System::EventHandler(this, &Form1::zapiszP_Click);
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->groupBox2);
			this->tabPage1->Controls->Add(this->groupBox3);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(438, 494);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Obiekt";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->zmienDlZakl);
			this->groupBox2->Controls->Add(this->dlZakl);
			this->groupBox2->Controls->Add(this->label13);
			this->groupBox2->Controls->Add(this->zmienWszystko);
			this->groupBox2->Controls->Add(this->zmienI2);
			this->groupBox2->Controls->Add(this->i2);
			this->groupBox2->Controls->Add(this->zmienI1);
			this->groupBox2->Controls->Add(this->wyMin);
			this->groupBox2->Controls->Add(this->label12);
			this->groupBox2->Controls->Add(this->i1);
			this->groupBox2->Controls->Add(this->label11);
			this->groupBox2->Controls->Add(this->zmienD);
			this->groupBox2->Controls->Add(this->label10);
			this->groupBox2->Controls->Add(this->d);
			this->groupBox2->Controls->Add(this->zmienWyMin);
			this->groupBox2->Controls->Add(this->label7);
			this->groupBox2->Controls->Add(this->wyMax);
			this->groupBox2->Controls->Add(this->label8);
			this->groupBox2->Controls->Add(this->zmienWariancja);
			this->groupBox2->Controls->Add(this->label9);
			this->groupBox2->Controls->Add(this->zmienWyMax);
			this->groupBox2->Controls->Add(this->zmienC2);
			this->groupBox2->Controls->Add(this->wariancja);
			this->groupBox2->Controls->Add(this->c2);
			this->groupBox2->Controls->Add(this->zmienB2);
			this->groupBox2->Controls->Add(this->b2);
			this->groupBox2->Controls->Add(this->zmienA2);
			this->groupBox2->Controls->Add(this->a2);
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Controls->Add(this->label6);
			this->groupBox2->Controls->Add(this->zmienC1);
			this->groupBox2->Controls->Add(this->c1);
			this->groupBox2->Controls->Add(this->zmienB1);
			this->groupBox2->Controls->Add(this->b1);
			this->groupBox2->Controls->Add(this->zmienA1);
			this->groupBox2->Controls->Add(this->a1);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->label2);
			this->groupBox2->Controls->Add(this->label1);
			this->groupBox2->ForeColor = System::Drawing::Color::Black;
			this->groupBox2->Location = System::Drawing::Point(8, 6);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(417, 397);
			this->groupBox2->TabIndex = 9;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Zmiana parametrów";
			// 
			// zmienDlZakl
			// 
			this->zmienDlZakl->Location = System::Drawing::Point(303, 327);
			this->zmienDlZakl->Name = L"zmienDlZakl";
			this->zmienDlZakl->Size = System::Drawing::Size(46, 23);
			this->zmienDlZakl->TabIndex = 39;
			this->zmienDlZakl->Text = L"Zmieñ";
			this->zmienDlZakl->UseVisualStyleBackColor = true;
			// 
			// dlZakl
			// 
			this->dlZakl->Location = System::Drawing::Point(139, 329);
			this->dlZakl->Name = L"dlZakl";
			this->dlZakl->Size = System::Drawing::Size(158, 20);
			this->dlZakl->TabIndex = 38;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(28, 332);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(105, 13);
			this->label13->TabIndex = 37;
			this->label13->Text = L"d³ugoœæ zak³ócenia:";
			// 
			// zmienWszystko
			// 
			this->zmienWszystko->Location = System::Drawing::Point(226, 368);
			this->zmienWszystko->Name = L"zmienWszystko";
			this->zmienWszystko->Size = System::Drawing::Size(123, 23);
			this->zmienWszystko->TabIndex = 36;
			this->zmienWszystko->Text = L"Zmieñ wszystko";
			this->zmienWszystko->UseVisualStyleBackColor = true;
			// 
			// zmienI2
			// 
			this->zmienI2->Location = System::Drawing::Point(303, 223);
			this->zmienI2->Name = L"zmienI2";
			this->zmienI2->Size = System::Drawing::Size(46, 23);
			this->zmienI2->TabIndex = 35;
			this->zmienI2->Text = L"Zmieñ";
			this->zmienI2->UseVisualStyleBackColor = true;
			// 
			// i2
			// 
			this->i2->Location = System::Drawing::Point(139, 225);
			this->i2->Name = L"i2";
			this->i2->Size = System::Drawing::Size(158, 20);
			this->i2->TabIndex = 34;
			// 
			// zmienI1
			// 
			this->zmienI1->Location = System::Drawing::Point(303, 197);
			this->zmienI1->Name = L"zmienI1";
			this->zmienI1->Size = System::Drawing::Size(46, 23);
			this->zmienI1->TabIndex = 33;
			this->zmienI1->Text = L"Zmieñ";
			this->zmienI1->UseVisualStyleBackColor = true;
			// 
			// wyMin
			// 
			this->wyMin->Location = System::Drawing::Point(139, 251);
			this->wyMin->Name = L"wyMin";
			this->wyMin->Size = System::Drawing::Size(158, 20);
			this->wyMin->TabIndex = 21;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(29, 254);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(40, 13);
			this->label12->TabIndex = 18;
			this->label12->Text = L"wyMin:";
			// 
			// i1
			// 
			this->i1->Location = System::Drawing::Point(139, 199);
			this->i1->Name = L"i1";
			this->i1->Size = System::Drawing::Size(158, 20);
			this->i1->TabIndex = 32;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(28, 280);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(43, 13);
			this->label11->TabIndex = 19;
			this->label11->Text = L"wyMax:";
			// 
			// zmienD
			// 
			this->zmienD->Location = System::Drawing::Point(303, 171);
			this->zmienD->Name = L"zmienD";
			this->zmienD->Size = System::Drawing::Size(46, 23);
			this->zmienD->TabIndex = 31;
			this->zmienD->Text = L"Zmieñ";
			this->zmienD->UseVisualStyleBackColor = true;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(28, 306);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(55, 13);
			this->label10->TabIndex = 20;
			this->label10->Text = L"wariancja:";
			// 
			// d
			// 
			this->d->Location = System::Drawing::Point(139, 173);
			this->d->Name = L"d";
			this->d->Size = System::Drawing::Size(158, 20);
			this->d->TabIndex = 30;
			// 
			// zmienWyMin
			// 
			this->zmienWyMin->Location = System::Drawing::Point(303, 249);
			this->zmienWyMin->Name = L"zmienWyMin";
			this->zmienWyMin->Size = System::Drawing::Size(46, 23);
			this->zmienWyMin->TabIndex = 22;
			this->zmienWyMin->Text = L"Zmieñ";
			this->zmienWyMin->UseVisualStyleBackColor = true;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(28, 228);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(19, 13);
			this->label7->TabIndex = 29;
			this->label7->Text = L"I2:";
			// 
			// wyMax
			// 
			this->wyMax->Location = System::Drawing::Point(139, 277);
			this->wyMax->Name = L"wyMax";
			this->wyMax->Size = System::Drawing::Size(158, 20);
			this->wyMax->TabIndex = 23;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(28, 202);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(19, 13);
			this->label8->TabIndex = 28;
			this->label8->Text = L"I1:";
			// 
			// zmienWariancja
			// 
			this->zmienWariancja->Location = System::Drawing::Point(303, 301);
			this->zmienWariancja->Name = L"zmienWariancja";
			this->zmienWariancja->Size = System::Drawing::Size(46, 23);
			this->zmienWariancja->TabIndex = 26;
			this->zmienWariancja->Text = L"Zmieñ";
			this->zmienWariancja->UseVisualStyleBackColor = true;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(29, 176);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(16, 13);
			this->label9->TabIndex = 27;
			this->label9->Text = L"d:";
			// 
			// zmienWyMax
			// 
			this->zmienWyMax->Location = System::Drawing::Point(303, 275);
			this->zmienWyMax->Name = L"zmienWyMax";
			this->zmienWyMax->Size = System::Drawing::Size(46, 23);
			this->zmienWyMax->TabIndex = 24;
			this->zmienWyMax->Text = L"Zmieñ";
			this->zmienWyMax->UseVisualStyleBackColor = true;
			// 
			// zmienC2
			// 
			this->zmienC2->Location = System::Drawing::Point(303, 145);
			this->zmienC2->Name = L"zmienC2";
			this->zmienC2->Size = System::Drawing::Size(46, 23);
			this->zmienC2->TabIndex = 17;
			this->zmienC2->Text = L"Zmieñ";
			this->zmienC2->UseVisualStyleBackColor = true;
			// 
			// wariancja
			// 
			this->wariancja->Location = System::Drawing::Point(139, 303);
			this->wariancja->Name = L"wariancja";
			this->wariancja->Size = System::Drawing::Size(158, 20);
			this->wariancja->TabIndex = 25;
			// 
			// c2
			// 
			this->c2->Location = System::Drawing::Point(139, 147);
			this->c2->Name = L"c2";
			this->c2->Size = System::Drawing::Size(158, 20);
			this->c2->TabIndex = 16;
			// 
			// zmienB2
			// 
			this->zmienB2->Location = System::Drawing::Point(303, 119);
			this->zmienB2->Name = L"zmienB2";
			this->zmienB2->Size = System::Drawing::Size(46, 23);
			this->zmienB2->TabIndex = 15;
			this->zmienB2->Text = L"Zmieñ";
			this->zmienB2->UseVisualStyleBackColor = true;
			// 
			// b2
			// 
			this->b2->Location = System::Drawing::Point(139, 121);
			this->b2->Name = L"b2";
			this->b2->Size = System::Drawing::Size(158, 20);
			this->b2->TabIndex = 14;
			// 
			// zmienA2
			// 
			this->zmienA2->Location = System::Drawing::Point(303, 93);
			this->zmienA2->Name = L"zmienA2";
			this->zmienA2->Size = System::Drawing::Size(46, 23);
			this->zmienA2->TabIndex = 13;
			this->zmienA2->Text = L"Zmieñ";
			this->zmienA2->UseVisualStyleBackColor = true;
			// 
			// a2
			// 
			this->a2->Location = System::Drawing::Point(139, 95);
			this->a2->Name = L"a2";
			this->a2->Size = System::Drawing::Size(158, 20);
			this->a2->TabIndex = 12;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(28, 150);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(23, 13);
			this->label4->TabIndex = 11;
			this->label4->Text = L"C2:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(28, 124);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(23, 13);
			this->label5->TabIndex = 10;
			this->label5->Text = L"B2:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(29, 98);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(23, 13);
			this->label6->TabIndex = 9;
			this->label6->Text = L"A2:";
			// 
			// zmienC1
			// 
			this->zmienC1->Location = System::Drawing::Point(303, 67);
			this->zmienC1->Name = L"zmienC1";
			this->zmienC1->Size = System::Drawing::Size(46, 23);
			this->zmienC1->TabIndex = 8;
			this->zmienC1->Text = L"Zmieñ";
			this->zmienC1->UseVisualStyleBackColor = true;
			// 
			// c1
			// 
			this->c1->Location = System::Drawing::Point(139, 69);
			this->c1->Name = L"c1";
			this->c1->Size = System::Drawing::Size(158, 20);
			this->c1->TabIndex = 7;
			// 
			// zmienB1
			// 
			this->zmienB1->Location = System::Drawing::Point(303, 41);
			this->zmienB1->Name = L"zmienB1";
			this->zmienB1->Size = System::Drawing::Size(46, 23);
			this->zmienB1->TabIndex = 6;
			this->zmienB1->Text = L"Zmieñ";
			this->zmienB1->UseVisualStyleBackColor = true;
			// 
			// b1
			// 
			this->b1->Location = System::Drawing::Point(139, 43);
			this->b1->Name = L"b1";
			this->b1->Size = System::Drawing::Size(158, 20);
			this->b1->TabIndex = 5;
			// 
			// zmienA1
			// 
			this->zmienA1->Location = System::Drawing::Point(303, 15);
			this->zmienA1->Name = L"zmienA1";
			this->zmienA1->Size = System::Drawing::Size(46, 23);
			this->zmienA1->TabIndex = 4;
			this->zmienA1->Text = L"Zmieñ";
			this->zmienA1->UseVisualStyleBackColor = true;
			// 
			// a1
			// 
			this->a1->Location = System::Drawing::Point(139, 13);
			this->a1->Name = L"a1";
			this->a1->Size = System::Drawing::Size(158, 20);
			this->a1->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(28, 72);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(23, 13);
			this->label3->TabIndex = 2;
			this->label3->Text = L"C1:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(28, 46);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(23, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"B1:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(29, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(23, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"A1:";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->wczytaj);
			this->groupBox3->Controls->Add(this->label14);
			this->groupBox3->Controls->Add(this->nazwaObiektu);
			this->groupBox3->ForeColor = System::Drawing::Color::Black;
			this->groupBox3->Location = System::Drawing::Point(6, 409);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(417, 79);
			this->groupBox3->TabIndex = 10;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Wczytaj now¹ konfiguracjê";
			// 
			// wczytaj
			// 
			this->wczytaj->Location = System::Drawing::Point(277, 45);
			this->wczytaj->Name = L"wczytaj";
			this->wczytaj->Size = System::Drawing::Size(75, 23);
			this->wczytaj->TabIndex = 2;
			this->wczytaj->Text = L"Wczytaj";
			this->wczytaj->UseVisualStyleBackColor = true;
			this->wczytaj->Click += gcnew System::EventHandler(this, &Form1::wczytaj_Click);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(31, 22);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(81, 13);
			this->label14->TabIndex = 1;
			this->label14->Text = L"Nazwa obiektu:";
			// 
			// nazwaObiektu
			// 
			this->nazwaObiektu->Location = System::Drawing::Point(118, 19);
			this->nazwaObiektu->Name = L"nazwaObiektu";
			this->nazwaObiektu->Size = System::Drawing::Size(234, 20);
			this->nazwaObiektu->TabIndex = 0;
			// 
			// tabControl
			// 
			this->tabControl->Controls->Add(this->tabPage1);
			this->tabControl->Controls->Add(this->tabPage2);
			this->tabControl->Controls->Add(this->tabPage8);
			this->tabControl->Controls->Add(this->tabPage18);
			this->tabControl->Controls->Add(this->tabPage19);
			this->tabControl->Controls->Add(this->tabPage82);
			this->tabControl->Location = System::Drawing::Point(463, 148);
			this->tabControl->Name = L"tabControl";
			this->tabControl->SelectedIndex = 0;
			this->tabControl->Size = System::Drawing::Size(446, 520);
			this->tabControl->TabIndex = 11;
			this->tabControl->Click += gcnew System::EventHandler(this, &Form1::zmienWyMinGPC_Click);
			// 
			// wykres2
			// 
			this->wykres2->BackColor = System::Drawing::SystemColors::Control;
			this->wykres2->Location = System::Drawing::Point(13, 339);
			this->wykres2->Name = L"wykres2";
			this->wykres2->Size = System::Drawing::Size(444, 325);
			this->wykres2->TabIndex = 2;
			// 
			// wykres1
			// 
			this->wykres1->BackColor = System::Drawing::SystemColors::Control;
			this->wykres1->Location = System::Drawing::Point(13, 12);
			this->wykres1->Name = L"wykres1";
			this->wykres1->Size = System::Drawing::Size(444, 325);
			this->wykres1->TabIndex = 0;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(913, 680);
			this->Controls->Add(this->typRegulatora);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->reset);
			this->Controls->Add(this->stop);
			this->Controls->Add(this->wykres2);
			this->Controls->Add(this->start);
			this->Controls->Add(this->wykres1);
			this->Controls->Add(this->tabControl);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->czasKrokuTrackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->czasKrokuNumericUpDown))->EndInit();
			this->tabPage28->ResumeLayout(false);
			this->groupBox10->ResumeLayout(false);
			this->groupBox10->PerformLayout();
			this->groupBox11->ResumeLayout(false);
			this->groupBox11->PerformLayout();
			this->tabPage29->ResumeLayout(false);
			this->tabPage29->PerformLayout();
			this->tabControl8->ResumeLayout(false);
			this->tabPage30->ResumeLayout(false);
			this->tabPage30->PerformLayout();
			this->tabPage31->ResumeLayout(false);
			this->groupBox12->ResumeLayout(false);
			this->groupBox12->PerformLayout();
			this->tabControl9->ResumeLayout(false);
			this->tabPage32->ResumeLayout(false);
			this->tabPage32->PerformLayout();
			this->tabPage33->ResumeLayout(false);
			this->tabPage33->PerformLayout();
			this->tabPage34->ResumeLayout(false);
			this->tabPage34->PerformLayout();
			this->tabPage35->ResumeLayout(false);
			this->tabPage35->PerformLayout();
			this->tabPage36->ResumeLayout(false);
			this->tabPage36->PerformLayout();
			this->groupBox13->ResumeLayout(false);
			this->groupBox13->PerformLayout();
			this->tabPage37->ResumeLayout(false);
			this->tabPage37->PerformLayout();
			this->tabControl10->ResumeLayout(false);
			this->tabPage38->ResumeLayout(false);
			this->tabPage38->PerformLayout();
			this->tabPage39->ResumeLayout(false);
			this->groupBox14->ResumeLayout(false);
			this->groupBox14->PerformLayout();
			this->tabControl11->ResumeLayout(false);
			this->tabPage40->ResumeLayout(false);
			this->tabPage40->PerformLayout();
			this->tabPage41->ResumeLayout(false);
			this->tabPage41->PerformLayout();
			this->tabPage42->ResumeLayout(false);
			this->tabPage42->PerformLayout();
			this->tabPage43->ResumeLayout(false);
			this->tabPage43->PerformLayout();
			this->tabPage44->ResumeLayout(false);
			this->tabPage44->PerformLayout();
			this->groupBox15->ResumeLayout(false);
			this->groupBox15->PerformLayout();
			this->tabPage45->ResumeLayout(false);
			this->tabPage45->PerformLayout();
			this->tabControl12->ResumeLayout(false);
			this->tabPage46->ResumeLayout(false);
			this->tabPage46->PerformLayout();
			this->tabPage47->ResumeLayout(false);
			this->groupBox16->ResumeLayout(false);
			this->groupBox16->PerformLayout();
			this->tabControl13->ResumeLayout(false);
			this->tabPage48->ResumeLayout(false);
			this->tabPage48->PerformLayout();
			this->tabPage49->ResumeLayout(false);
			this->tabPage49->PerformLayout();
			this->tabPage50->ResumeLayout(false);
			this->tabPage50->PerformLayout();
			this->tabPage51->ResumeLayout(false);
			this->tabPage51->PerformLayout();
			this->tabPage52->ResumeLayout(false);
			this->tabPage52->PerformLayout();
			this->tabPage53->ResumeLayout(false);
			this->tabPage53->PerformLayout();
			this->groupBox17->ResumeLayout(false);
			this->groupBox17->PerformLayout();
			this->tabPage54->ResumeLayout(false);
			this->tabPage54->PerformLayout();
			this->tabPage55->ResumeLayout(false);
			this->groupBox18->ResumeLayout(false);
			this->groupBox18->PerformLayout();
			this->groupBox19->ResumeLayout(false);
			this->groupBox19->PerformLayout();
			this->tabPage56->ResumeLayout(false);
			this->tabPage56->PerformLayout();
			this->tabControl14->ResumeLayout(false);
			this->tabPage57->ResumeLayout(false);
			this->tabPage57->PerformLayout();
			this->tabPage58->ResumeLayout(false);
			this->groupBox20->ResumeLayout(false);
			this->groupBox20->PerformLayout();
			this->tabControl15->ResumeLayout(false);
			this->tabPage59->ResumeLayout(false);
			this->tabPage59->PerformLayout();
			this->tabPage60->ResumeLayout(false);
			this->tabPage60->PerformLayout();
			this->tabPage61->ResumeLayout(false);
			this->tabPage61->PerformLayout();
			this->tabPage62->ResumeLayout(false);
			this->tabPage62->PerformLayout();
			this->tabPage63->ResumeLayout(false);
			this->tabPage63->PerformLayout();
			this->groupBox21->ResumeLayout(false);
			this->groupBox21->PerformLayout();
			this->tabPage64->ResumeLayout(false);
			this->tabPage64->PerformLayout();
			this->tabControl16->ResumeLayout(false);
			this->tabPage65->ResumeLayout(false);
			this->tabPage65->PerformLayout();
			this->tabPage66->ResumeLayout(false);
			this->groupBox22->ResumeLayout(false);
			this->groupBox22->PerformLayout();
			this->tabControl17->ResumeLayout(false);
			this->tabPage67->ResumeLayout(false);
			this->tabPage67->PerformLayout();
			this->tabPage68->ResumeLayout(false);
			this->tabPage68->PerformLayout();
			this->tabPage69->ResumeLayout(false);
			this->tabPage69->PerformLayout();
			this->tabPage70->ResumeLayout(false);
			this->tabPage70->PerformLayout();
			this->tabPage71->ResumeLayout(false);
			this->tabPage71->PerformLayout();
			this->groupBox23->ResumeLayout(false);
			this->groupBox23->PerformLayout();
			this->tabPage72->ResumeLayout(false);
			this->tabPage72->PerformLayout();
			this->tabControl18->ResumeLayout(false);
			this->tabPage73->ResumeLayout(false);
			this->tabPage73->PerformLayout();
			this->tabPage74->ResumeLayout(false);
			this->groupBox24->ResumeLayout(false);
			this->groupBox24->PerformLayout();
			this->tabControl19->ResumeLayout(false);
			this->tabPage75->ResumeLayout(false);
			this->tabPage75->PerformLayout();
			this->tabPage76->ResumeLayout(false);
			this->tabPage76->PerformLayout();
			this->tabPage77->ResumeLayout(false);
			this->tabPage77->PerformLayout();
			this->tabPage78->ResumeLayout(false);
			this->tabPage78->PerformLayout();
			this->tabPage79->ResumeLayout(false);
			this->tabPage79->PerformLayout();
			this->tabPage80->ResumeLayout(false);
			this->tabPage80->PerformLayout();
			this->groupBox25->ResumeLayout(false);
			this->groupBox25->PerformLayout();
			this->tabPage81->ResumeLayout(false);
			this->tabPage81->PerformLayout();
			this->tabPage82->ResumeLayout(false);
			this->tabPage82->PerformLayout();
			this->tabControl7->ResumeLayout(false);
			this->tabPage83->ResumeLayout(false);
			this->tabPage83->PerformLayout();
			this->tabPage84->ResumeLayout(false);
			this->groupBox26->ResumeLayout(false);
			this->groupBox26->PerformLayout();
			this->tabPage999->ResumeLayout(false);
			this->tabPage85->ResumeLayout(false);
			this->tabPage85->PerformLayout();
			this->tabPage86->ResumeLayout(false);
			this->tabPage86->PerformLayout();
			this->tabPage87->ResumeLayout(false);
			this->tabPage87->PerformLayout();
			this->tabPage88->ResumeLayout(false);
			this->tabPage88->PerformLayout();
			this->tabPage89->ResumeLayout(false);
			this->tabPage89->PerformLayout();
			this->tabPage19->ResumeLayout(false);
			this->tabPage19->PerformLayout();
			this->tabPage18->ResumeLayout(false);
			this->tabPage18->PerformLayout();
			this->tabControl5->ResumeLayout(false);
			this->tabPage20->ResumeLayout(false);
			this->tabPage20->PerformLayout();
			this->tabPage21->ResumeLayout(false);
			this->groupBox8->ResumeLayout(false);
			this->groupBox8->PerformLayout();
			this->tabControl6->ResumeLayout(false);
			this->tabPage22->ResumeLayout(false);
			this->tabPage22->PerformLayout();
			this->tabPage23->ResumeLayout(false);
			this->tabPage23->PerformLayout();
			this->tabPage24->ResumeLayout(false);
			this->tabPage24->PerformLayout();
			this->tabPage25->ResumeLayout(false);
			this->tabPage25->PerformLayout();
			this->tabPage26->ResumeLayout(false);
			this->tabPage26->PerformLayout();
			this->tabPage27->ResumeLayout(false);
			this->tabPage27->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->tabPage8->ResumeLayout(false);
			this->tabPage8->PerformLayout();
			this->tabControl3->ResumeLayout(false);
			this->tabPage14->ResumeLayout(false);
			this->tabPage14->PerformLayout();
			this->tabPage15->ResumeLayout(false);
			this->groupBox9->ResumeLayout(false);
			this->groupBox9->PerformLayout();
			this->tabControl2->ResumeLayout(false);
			this->tabPage9->ResumeLayout(false);
			this->tabPage9->PerformLayout();
			this->tabPage10->ResumeLayout(false);
			this->tabPage10->PerformLayout();
			this->tabPage11->ResumeLayout(false);
			this->tabPage11->PerformLayout();
			this->tabPage12->ResumeLayout(false);
			this->tabPage12->PerformLayout();
			this->tabPage13->ResumeLayout(false);
			this->tabPage13->PerformLayout();
			this->groupBox7->ResumeLayout(false);
			this->groupBox7->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->tabControl4->ResumeLayout(false);
			this->tabPage16->ResumeLayout(false);
			this->tabPage16->PerformLayout();
			this->tabPage17->ResumeLayout(false);
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			this->tabControl1->ResumeLayout(false);
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			this->tabPage4->ResumeLayout(false);
			this->tabPage4->PerformLayout();
			this->tabPage5->ResumeLayout(false);
			this->tabPage5->PerformLayout();
			this->tabPage6->ResumeLayout(false);
			this->tabPage6->PerformLayout();
			this->tabPage7->ResumeLayout(false);
			this->tabPage7->PerformLayout();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->tabPage1->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->tabControl->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void start_Click(System::Object^  sender, System::EventArgs^  e)
	{
		start->Enabled = false;
		stop->Enabled = true;

		Thread^ sym = gcnew Thread( gcnew ThreadStart(this,&PSS_LAB::Form1::symulacja));
		
		sym->Start();

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::start, 1);
	}

	private: ArrayList w;
	private: ArrayList wy;
	private: ArrayList sterowanie;
	private: armax *model;
	private: RegulatorP *reg;
	private: RegulatorPID *regPID;
	private: AdaptPID *regAPID;
	private: GPC *regGPC;
	private: PetlaRegulacji *petla;
	private: Identyfikacja *ident;
	private: int krok;
	private: int krokGPC;
	private: double alfa;
	private: double alfaaGPC;
	private: bool regNieGPC;
	private: bool isAPID;
	private: ServerSocket *serwer;
	private: static const int PORT = 76543;
	private: int iloscKlientow;
	private: SOCKET *dyskryptory;

	private: double Wyj;
	private: double Ster;
	private: double WarZad;	

	private: float naszaDana;
	private: TYP typRozkazu;
	private: SOCKET nrDyskryptora;

	private: Autentykacja *autentyk;
	
	private: System::Void nowyKlient()
	{
		SOCKET deskrPolaczenia;
		dyskryptory = new SOCKET[100];
		
		while(true)
		{
			deskrPolaczenia = serwer->Accept();
			dyskryptory[iloscKlientow] = deskrPolaczenia;
	
			Thread^ odbklient = gcnew Thread( gcnew ThreadStart(this,&PSS_LAB::Form1::OdbierzOdKlienta));
			odbklient->Start();
		}
       
	}

	private:delegate System::Void wyslijTypRegulatora_d();
	private:delegate System::Void zmienParametry_d();			

	private: System::Void OdbierzOdKlienta()
	{

		SOCKET deskrPolaczenia = dyskryptory[iloscKlientow];
		iloscKlientow++;
		
		ClientSocket klientNew = deskrPolaczenia;
		Serializacja serialNew;

		string nazwaUz, hasloUz;

		char *recMessage = new char[serialNew.ROZMIAR];

		while(true)
		{	
			int Result = klientNew.RecvData( recMessage, serialNew.ROZMIAR );
			
			nrDyskryptora = deskrPolaczenia;

			if( (Result == SOCKET_ERROR) || (Result == 0))
			{
				
				for(int i=0;i<iloscKlientow;i++)
				{
					if(dyskryptory[i] == deskrPolaczenia)
					{
						for(int j=i; j<iloscKlientow; j++)
							dyskryptory[j] = dyskryptory[j+1];
						break;
					}
				}
				iloscKlientow--;
				break;
			}
			else
			{
				wyslijTypRegulatora_d^ typReg = gcnew wyslijTypRegulatora_d(this, &PSS_LAB::Form1::wyslijTypRegulatora);
				zmienParametry_d^ zmParam = gcnew zmienParametry_d(this, &PSS_LAB::Form1::zmienParametry);

				try
				{
					serialNew.Dekoduj(recMessage);
							
					typRozkazu = serialNew.getRozkaz();
					naszaDana = serialNew.getDane();
				}
				catch(SocketException& exc)
				{
					System::Console::WriteLine(exc.what());
					MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
					//exit(1);
					continue;
				}	
				
				if(typRozkazu == TYP::nazwaUzytkownika)
					nazwaUz = serialNew.getNazwa();
				else if(typRozkazu == TYP::hasloUzytkownika)
				{
					char buf ;
					stringstream ss(nazwaUz); 
					nazwaUz.clear();
					while (ss >> buf) nazwaUz += buf ; 

					hasloUz = serialNew.getNazwa();
					stringstream ss1(hasloUz);
					hasloUz.clear();
					while (ss1 >> buf) hasloUz += buf ;

					int dos = static_cast<int> (autentyk->getPrawaDostepu(nazwaUz,hasloUz)); 
					WyslijDoKlienta(deskrPolaczenia, TYP::typDostepu, dos );
				}

				// jesli klient wyslal swoje nastawy
				
				else if(naszaDana != -9999)
					this->Invoke(zmParam);

				// jesli klient wyslal ¿¹danie wyslania nastaw serwera

				// typ regulatora
				else if(typRozkazu == TYP::typRegulatora)
					this->Invoke(typReg);
				
				// do wykresu
				else if(typRozkazu == TYP::wyjscieObiektu)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, Wyj );
				else if(typRozkazu == TYP::sterowanie)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, Ster);
				else if(typRozkazu == TYP::wartoscZadana)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, WarZad );
				else if(typRozkazu == TYP::czasKrokuTrackBar)
					this->Invoke(zmParam);
				else if(typRozkazu == TYP::czasKrokuNumericUpDown)
					this->Invoke(zmParam);
								
				// do regulatora P
				else if(typRozkazu == TYP::k)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getK() );
				else if(typRozkazu == TYP::wyMaxP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getWyMax() );
				else if(typRozkazu == TYP::wyMinP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getWyMin() );
				else if(typRozkazu == TYP::nowySkokP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getSkok() );
				else if(typRozkazu == TYP::nowaAmplitudaPP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getProstokatAmplituda() );
				else if(typRozkazu == TYP::nowyOkresPP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getProstokatOkres() );
				else if(typRozkazu == TYP::noweWypelnieniePP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getProstokatWypelnienie() );
				else if(typRozkazu == TYP::nowaAmplitudaSP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getSinusAmplituda() );
				else if(typRozkazu == TYP::nowyOkresSP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getSinusOkres() );
				else if(typRozkazu == TYP::nowaAmplitudaTP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getTrojkatAmplituda() );
				else if(typRozkazu == TYP::nowyOkresTP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getTrojkatOkres() );
				else if(typRozkazu == TYP::nowaWariancjaP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getSzumWariancja() );
				else if(typRozkazu == TYP::nowaLiczbaProbekP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getSzumDlZakl() );
				else if(typRozkazu == TYP::skokP)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, reg->getSygnal() );

				// do regulatora PID
				else if(typRozkazu == TYP::kPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getK() );
				else if(typRozkazu == TYP::tiPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getTi() );
				else if(typRozkazu == TYP::tdPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getTd() );
				else if(typRozkazu == TYP::tpPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getTp() );
				else if(typRozkazu == TYP::wpPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getWarPocz() );
				else if(typRozkazu == TYP::nPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getN() );
				else if(typRozkazu == TYP::bPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getB() );
				else if(typRozkazu == TYP::wyMaxPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getWyMax() );
				else if(typRozkazu == TYP::wyMinPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getWyMin() );
				else if(typRozkazu == TYP::nowySkokPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getSkok() );
				else if(typRozkazu == TYP::nowaAmplitudaPPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getProstokatAmplituda() );
				else if(typRozkazu == TYP::nowyOkresPPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getProstokatOkres() );
				else if(typRozkazu == TYP::noweWypelnieniePPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getProstokatWypelnienie() );
				else if(typRozkazu == TYP::nowaAmplitudaSPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getSinusAmplituda() );
				else if(typRozkazu == TYP::nowyOkresSPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getSinusOkres() );
				else if(typRozkazu == TYP::nowaAmplitudaTPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getTrojkatAmplituda() );
				else if(typRozkazu == TYP::nowyOkresTPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getTrojkatOkres() );
				else if(typRozkazu == TYP::nowaWariancjaPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getSzumWariancja() );
				else if(typRozkazu == TYP::nowaLiczbaProbekPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getSzumDlZakl() );
				else if(typRozkazu == TYP::skokPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regPID->getSygnal() );

				// do regulatora GPC
				else if(typRozkazu == TYP::HGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getH() );
				else if(typRozkazu == TYP::LGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getL() );
				else if(typRozkazu == TYP::alfaGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getAlfa() );
				else if(typRozkazu == TYP::roGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getRo() );
				else if(typRozkazu == TYP::dAGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getDA() );
				else if(typRozkazu == TYP::dBGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getDB() );
				else if(typRozkazu == TYP::wyMaxGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getWyMax() );
				else if(typRozkazu == TYP::wyMinGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getWyMin() );
				else if(typRozkazu == TYP::nowySkokGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getSkok() );
				else if(typRozkazu == TYP::nowaAmplitudaPGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getProstokatAmplituda() );
				else if(typRozkazu == TYP::nowyOkresPGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getProstokatOkres() );
				else if(typRozkazu == TYP::noweWypelnieniePGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getProstokatWypelnienie() );
				else if(typRozkazu == TYP::nowaAmplitudaSGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getSinusAmplituda() );
				else if(typRozkazu == TYP::nowyOkresSGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getSinusOkres() );
				else if(typRozkazu == TYP::nowaAmplitudaTGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getTrojkatAmplituda() );
				else if(typRozkazu == TYP::nowyOkresTGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getTrojkatOkres() );
				else if(typRozkazu == TYP::nowaWariancjaGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getSzumWariancja() );
				else if(typRozkazu == TYP::nowaLiczbaProbekGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getSzumDlZakl() );
				else if(typRozkazu == TYP::skokGPC)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regGPC->getSygnal() );
				else if(typRozkazu == TYP::nowaAlfaGPC)
				{
					char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAlfaGPC->Text);
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, strtod(cStr, NULL));
				}
				// do regulatora APID
				else if(typRozkazu == TYP::kAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getK() );
				else if(typRozkazu == TYP::tiAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getTi() );
				else if(typRozkazu == TYP::tdAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getTd() );
				else if(typRozkazu == TYP::onAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getOn() );
				else if(typRozkazu == TYP::wyMaxAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getWyMax() );
				else if(typRozkazu == TYP::wyMinAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getWyMin() );
				else if(typRozkazu == TYP::nowySkokAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getSkok() );
				else if(typRozkazu == TYP::nowaAmplitudaPAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getProstokatAmplituda() );
				else if(typRozkazu == TYP::nowyOkresPAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getProstokatOkres() );
				else if(typRozkazu == TYP::noweWypelnieniePAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getProstokatWypelnienie() );
				else if(typRozkazu == TYP::nowaAmplitudaSAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getSinusAmplituda() );
				else if(typRozkazu == TYP::nowyOkresSAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getSinusOkres() );
				else if(typRozkazu == TYP::nowaAmplitudaTAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getTrojkatAmplituda() );
				else if(typRozkazu == TYP::nowyOkresTAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getTrojkatOkres() );
				else if(typRozkazu == TYP::nowaWariancjaAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getSzumWariancja() );
				else if(typRozkazu == TYP::nowaLiczbaProbekAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getSzumDlZakl() );
				else if(typRozkazu == TYP::skokAPID)
					WyslijDoKlienta(deskrPolaczenia, typRozkazu, regAPID->getSygnal() );
			}
			
		}
		recMessage = NULL;
	}

	private: System::Void wyslijTypRegulatora()
	{
		WyslijDoKlienta(nrDyskryptora, TYP::typRegulatora, typRegulatora->SelectedIndex);
	}

	private: System::Void zmienParametry()
	{
		System::Object^  sender1;
		System::EventArgs^  e1;

		// do typu regulatora
		if(typRozkazu == TYP::typRegulatora)
		{
			typRegulatora->SelectedIndex = naszaDana;
			typRegulatora_SelectedIndexChanged(sender1,e1);
		}
		
		// do rysowania wykresu
		else if(typRozkazu == TYP::czasKrokuTrackBar)
		{
			if(naszaDana == -9999)
				WyslijDoKlienta(nrDyskryptora, typRozkazu, czasKrokuTrackBar->Value);
			else
			{
				czasKrokuTrackBar->Value = naszaDana;
				czasKrokuNumericUpDown_ValueChanged(sender1,e1);
			}
		}
		else if(typRozkazu == TYP::czasKrokuNumericUpDown)
		{
			if(naszaDana == -9999)
				WyslijDoKlienta(nrDyskryptora, typRozkazu, (int)czasKrokuNumericUpDown->Value);
			else
			{
				czasKrokuNumericUpDown->Value = (int) naszaDana;
				czasKrokuTrackBar_Scroll(sender1,e1);
			}
		}
		else if(typRozkazu == TYP::stop)
		{
			if(!stop->Checked)
					stop->Checked = true;
	
		}
		else if(typRozkazu == TYP::start)
		{
			if(start->Enabled)
				start_Click(sender1,e1);
		}

		// do regulatora P
		else if(typRozkazu == TYP::k)
		{
			k->Text = "" + naszaDana;
			zmienP_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::wyMaxP)
		{
			wyMaxP->Text = "" + naszaDana;
			zmienWyMaxP_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::wyMinP)
		{
			wyMinP->Text = "" + naszaDana;
			zmienWyMinP_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::nowySkokP)
			nowySkokP->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaPP)
			nowaAmplitudaPP->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresPP)
			nowyOkresPP->Text = "" + naszaDana;
		else if(typRozkazu == TYP::noweWypelnieniePP)
			noweWypelnieniePP->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaSP)
			nowaAmplitudaSP->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresSP)
			nowyOkresSP->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaTP)
			nowaAmplitudaTP->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresTP)
			nowyOkresTP->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaWariancjaP)
			nowaWariancjaP->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaLiczbaProbekP)
		{
			nowaLiczbaProbekP->Text = "" + naszaDana;
			zmienWymuszenieP_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::skokP)
		{
			reg->setSygnal(naszaDana);
			wyswietlParametryP();
		}

		// do regulatora PID
		else if(typRozkazu == TYP::kPID)
		{
			kPID->Text = "" + naszaDana;
			zmienKPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::tiPID)
		{
			tiPID->Text = "" + naszaDana;
			zmienTiPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::tdPID)
		{
			tdPID->Text = "" + naszaDana;
			zmienTdPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::tpPID)
		{
			tpPID->Text = "" + naszaDana;
			zmienTpPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::wpPID)
		{
			wpPID->Text = "" + naszaDana;
			zmienWpPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::nPID)
		{
			nPID->Text = "" + naszaDana;
			zmienNPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::bPID)
		{
			bPID->Text = "" + naszaDana;
			zmienBPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::wyMaxPID)
		{
			wyMaxPID->Text = "" + naszaDana;
			zmienWyMaxPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::wyMinPID)
		{
			wyMinPID->Text = "" + naszaDana;
			zmienWyMinPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::nowySkokPID)
			nowySkokPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaPPID)
			nowaAmplitudaPPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresPPID)
			nowyOkresPPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::noweWypelnieniePPID)
			noweWypelnieniePPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaSPID)
			nowaAmplitudaSPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresSPID)
			nowyOkresSPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaTPID)
			nowaAmplitudaTPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresTPID)
			nowyOkresTPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaWariancjaPID)
			nowaWariancjaPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaLiczbaProbekPID)
		{
			nowaLiczbaProbekPID->Text = "" + naszaDana;
			zmienWymuszeniePID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::skokPID)
		{
			regPID->setSygnal(naszaDana);
			wyswietlParametryPID();
		}

		// do regulatora GPC
		else if(typRozkazu == TYP::HGPC)
		{
			HGPC->Text = "" + naszaDana;
			zmienHGPC_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::LGPC)
		{
			LGPC->Text = "" + naszaDana;
			zmienLGPC_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::alfaGPC)
		{
			alfaGPC->Text = "" + naszaDana;
			zmienalfaGPC_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::roGPC)
		{
			roGPC->Text = "" + naszaDana;
			zmienroGPC_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::dAGPC)
		{
			dAGPC->Text = "" + naszaDana;
			zmiendAGPC_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::dBGPC)
		{
			dBGPC->Text = "" + naszaDana;
			zmiendBGPC_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::wyMaxGPC)
		{
			wyMaxGPC->Text = "" + naszaDana;
			zmienWyMaxGPC_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::wyMinGPC)
		{
			wyMinGPC->Text = "" + naszaDana;
			zmienWyMinGPC_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::nowySkokGPC)
			nowySkokGPC->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaPGPC)
			nowaAmplitudaPGPC->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresPGPC)
			nowyOkresPGPC->Text = "" + naszaDana;
		else if(typRozkazu == TYP::noweWypelnieniePGPC)
			noweWypelnieniePGPC->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaSGPC)
			nowaAmplitudaSGPC->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresSGPC)
			nowyOkresSGPC->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaTGPC)
			nowaAmplitudaTGPC->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresTGPC)
			nowyOkresTGPC->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaWariancjaGPC)
			nowaWariancjaGPC->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaLiczbaProbekGPC)
		{
			nowaLiczbaProbekGPC->Text = "" + naszaDana;
			zmienWymuszenieGPC_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::skokGPC)
		{
			regGPC->setSygnal(naszaDana);
			wyswietlParametryGPC();
		}
		else if(typRozkazu == TYP::nowaAlfaGPC)
		{
			nowaAlfaGPC->Text = "" + naszaDana;
			zmiennAlfaGPC_Click(sender1,e1);
		}

		// do regulatora APID
		else if(typRozkazu == TYP::ZmienNastawy)
		{
			if(naszaDana == -1)
			{
				if (ZmienNastawy->Checked == false)
					WyslijDoKlienta(nrDyskryptora, typRozkazu, 0 );
				else
					WyslijDoKlienta(nrDyskryptora, typRozkazu, 1 );
			}
			else if(naszaDana == 0)
			{
				ZmienNastawy->Checked = false;
				ekspAPID_Changed(sender1,e1);
			}
			else
			{	
				ZmienNastawy->Checked = true;
				ekspAPID_Changed(sender1,e1);
			}
			
			
		}
		else if(typRozkazu == TYP::kAPID)
		{
			kAPID->Text = "" + naszaDana;
			zmienKAPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::tiAPID)
		{
			tiAPID->Text = "" + naszaDana;
			zmienTiAPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::tdAPID)
		{
			tdAPID->Text = "" + naszaDana;
			zmienTdAPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::onAPID)
		{
			onAPID->Text = "" + naszaDana;
			zmienOnAPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::wyMaxAPID)
		{
			wyMaxAPID->Text = "" + naszaDana;
			zmienWyMaxAPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::wyMinAPID)
		{
			wyMinAPID->Text = "" + naszaDana;
			zmienWyMinAPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::nowySkokAPID)
			nowySkokAPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaPAPID)
			nowaAmplitudaPAPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresPAPID)
			nowyOkresPAPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::noweWypelnieniePAPID)
			noweWypelnieniePAPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaSAPID)
			nowaAmplitudaSAPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresSAPID)
			nowyOkresSAPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaAmplitudaTAPID)
			nowaAmplitudaTAPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowyOkresTAPID)
			nowyOkresTAPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaWariancjaAPID)
			nowaWariancjaAPID->Text = "" + naszaDana;
		else if(typRozkazu == TYP::nowaLiczbaProbekAPID)
		{
			nowaLiczbaProbekAPID->Text = "" + naszaDana;
			zmienWymuszenieAPID_Click(sender1,e1);
		}
		else if(typRozkazu == TYP::skokAPID)
		{
			regAPID->setSygnal(naszaDana);
			wyswietlParametryAPID();
		}
	
		
	}

	private: System::Void WyslijDoKlienta(SOCKET deskr, TYP roz, float dana )
	{
		ClientSocket klientNew = deskr;
		Serializacja serialNew;

		char *recMessage = new char[serialNew.ROZMIAR];
		recMessage = serialNew.Koduj(roz, dana);

		try
		{
			klientNew.GetAndSendMessage(recMessage);
			recMessage = NULL;
		}
		catch(SocketException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
			exit(1);
		}
	
	}
	
	private: System::Void symulacja()
	{
		dodajW_d^ dodaj = gcnew dodajW_d(this, &PSS_LAB::Form1::dodajW);
		odswierzWykresy_d^ odswWykr = gcnew odswierzWykresy_d(this, &PSS_LAB::Form1::odswierzWykresy);
		odswierzIdentyfikacja_d^ odswIdent = gcnew odswierzIdentyfikacja_d(this, &PSS_LAB::Form1::odswierzIdentyfikacja);
		odswierzIdentyfikacjaGPC_d^ odswIdent1 = gcnew odswierzIdentyfikacjaGPC_d(this, &PSS_LAB::Form1::odswierzIdentyfikacjaGPC);
		odswierzPrzyciski_d^ odswPrzyc = gcnew odswierzPrzyciski_d(this, &PSS_LAB::Form1::odswierzPrzyciski);
		
		int czasJednegoKroku = (int)czasKrokuNumericUpDown->Value;
		
		try
		{
			double wyj = 0;
			double ster;
			double warZad;

			if(czasJednegoKroku>0)
				do
				{

					//Thread::Sleep(czasJednegoKroku+50);
					Thread::Sleep(czasJednegoKroku);
					this->Invoke(odswWykr);
					
					wyj = petla->symuluj();
					ster = petla->getSterowanie();
					warZad = petla->getWartoscZadana();
					
					Wyj = wyj;
					Ster = ster;
					WarZad = warZad; 

					if(identyfikuj->Checked)
					{
						krok ++;
						ident->liczParam(ster, wyj);
						this->Invoke(odswIdent);
					}

					if( (!regAPID->isStrojenie()) && (ZmienNastawy->Checked))
					{
						System::Object^  sender1;
						System::EventArgs^  e1;
						ekspAPID_Changed(sender1,e1);
					}
					

					if(regNieGPC == false)
					{
						krokGPC++;
						this->Invoke(odswIdent1);
					}
					this->Invoke(dodaj, gcnew array <System::Object^>(3){warZad, wyj, ster});
					
				}while(!stop->Checked);

			else
			{
				wyj = petla->symuluj();
				ster = petla->getSterowanie();
				warZad = petla->getWartoscZadana();

				Wyj = wyj;
				Ster = ster;
				WarZad = warZad;

				if((!regAPID->isStrojenie()) && (ZmienNastawy->Checked))
				{
						System::Object^  sender2;
						System::EventArgs^  e2;
						ekspAPID_Changed(sender2,e2);
				}
				
				if(regNieGPC == false)
				{
					krokGPC++;
					this->Invoke(odswIdent1);					
			    }

				this->Invoke(dodaj, gcnew array <System::Object^>(3){warZad, wyj, ster});
				
			}

			this->Invoke(odswWykr);
			this->Invoke(odswPrzyc);
		}
		catch(OpenException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
			model = NULL;
			this->Invoke(odswPrzyc);
		}
		catch(ReadDataException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
			model = NULL;
			this->Invoke(odswPrzyc);
		}
	}

	private: System::Void dodajW(double _w, double _wy, double _sterowanie)
	{
		w.Add(_w);
		wy.Add(_wy);
		sterowanie.Add(_sterowanie);
	}
	private:delegate System::Void dodajW_d(double _w, double _wy, double _sterowanie);

	private: System::Void odswierzWykresy()
	{
		wykres1->odswierz(w.ToArray(), wy.ToArray());
		wykres2->odswierz(sterowanie.ToArray());
	}
	private:delegate System::Void odswierzWykresy_d();

	private: System::Void odswierzIdentyfikacja()
	{
		identyfikacja_textBox->AppendText("===================================================="
			+ System::Environment::NewLine);
		identyfikacja_textBox->AppendText("Wynik identyfikacji w kroku " + krok + System::Environment::NewLine);
		identyfikacja_textBox->AppendText("===================================================="
			+ System::Environment::NewLine);

		vector<double> parametry = ident->getParam();
		
		int dA = model->get_dA();
		int dB = model->get_dB();
		int dC = model->get_dC();

		identyfikacja_textBox->AppendText(" A = [ 1 ");
		for(int i=dB+1; i<=dA+dB; i++)
			identyfikacja_textBox->AppendText(parametry[i] + " ");
		identyfikacja_textBox->AppendText("]" + System::Environment::NewLine);

		identyfikacja_textBox->AppendText(" B = [ ");
		for(int i=0; i<=dB; i++)
			identyfikacja_textBox->AppendText(parametry[i] + " ");
		identyfikacja_textBox->AppendText("]" + System::Environment::NewLine);

		identyfikacja_textBox->AppendText(" C = [ 1 ");
		for(int i=dA+dB+1; i<=dA+dB+dC; i++)
			identyfikacja_textBox->AppendText(parametry[i] + " ");
		identyfikacja_textBox->AppendText("]" + System::Environment::NewLine);
	}
	private: System::Void odswierzIdentyfikacjaGPC()
	{
		identyfikacjaGPC_textBox->AppendText("===================================================="
			+ System::Environment::NewLine);
		identyfikacjaGPC_textBox->AppendText("Wynik identyfikacji w kroku " + krokGPC + System::Environment::NewLine);
		identyfikacjaGPC_textBox->AppendText("===================================================="
			+ System::Environment::NewLine);

		vector<double> parametry = regGPC->identyf->getParam();
		
		int dA = regGPC->getDA();
		int dB = regGPC->getDB();
		int dC = 0;

		identyfikacjaGPC_textBox->AppendText(" A = [ 1 ");
		for(int i=dB+1; i<=dA+dB; i++)
			identyfikacjaGPC_textBox->AppendText(parametry[i] + " ");
		identyfikacjaGPC_textBox->AppendText("]" + System::Environment::NewLine);

		identyfikacjaGPC_textBox->AppendText(" B = [ ");
		for(int i=0; i<=dB; i++)
			identyfikacjaGPC_textBox->AppendText(parametry[i] + " ");
		identyfikacjaGPC_textBox->AppendText("]" + System::Environment::NewLine);

		identyfikacjaGPC_textBox->AppendText(" C = [ 1 ");
		for(int i=dA+dB+1; i<=dA+dB+dC; i++)
			identyfikacjaGPC_textBox->AppendText(parametry[i] + " ");
		identyfikacjaGPC_textBox->AppendText("]" + System::Environment::NewLine);
	}

	private:delegate System::Void odswierzIdentyfikacja_d();
	private:delegate System::Void odswierzIdentyfikacjaGPC_d();

    
	private: System::Void odswierzPrzyciski()
	{
		start->Enabled = true;
		stop->Enabled = false;
		stop->Checked = false;
	}
	private:delegate System::Void odswierzPrzyciski_d();

	private: System::Void reset_Click(System::Object^  sender, System::EventArgs^  e)
	{
		w.Clear();
		wy.Clear();
		sterowanie.Clear();
		petla->reset();
		ident->reset();
		identyfikacja_textBox->Clear();
		identyfikacjaGPC_textBox->Clear();
		wykres1->reset();
		wykres2->reset();
		wlaczKontrolkiAPID();

		krok = 0;
		krokGPC = 0;
	}

	private: System::Void czasKrokuTrackBar_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		czasKrokuNumericUpDown->Value = czasKrokuTrackBar->Value;

		for(int i=0; i<iloscKlientow; i++)
			WyslijDoKlienta(dyskryptory[i], TYP::czasKrokuTrackBar, czasKrokuTrackBar->Value);
	}

	private: System::Void czasKrokuNumericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		czasKrokuTrackBar->Value = (int)czasKrokuNumericUpDown->Value;

		for(int i=0; i<iloscKlientow; i++)
			WyslijDoKlienta(dyskryptory[i], TYP::czasKrokuNumericUpDown, (int) czasKrokuNumericUpDown->Value);
	}

	// Obiekt ARMAX

	private: System::Void wyswietlParametryObiektu()
	{
		nazwaObiektu->Text = gcnew System::String(model->getNazwa().c_str());

		vector<double> wspolczynniki;

		wspolczynniki = model->getA1();
		a1->Text = "";
		for(int i=0; i<wspolczynniki.size()-1; i++)
			a1->AppendText(wspolczynniki[i] + " ");
		a1->AppendText(wspolczynniki[wspolczynniki.size()-1] + " ");

		wspolczynniki = model->getB1();
		b1->Text = "";
		for(int i=0; i<wspolczynniki.size()-1; i++)
			b1->AppendText(wspolczynniki[i] + " ");
		b1->AppendText(wspolczynniki[wspolczynniki.size()-1] + " ");

		wspolczynniki = model->getC1();
		c1->Text = "";
		for(int i=0; i<wspolczynniki.size()-1; i++)
			c1->AppendText(wspolczynniki[i] + " ");
		c1->AppendText(wspolczynniki[wspolczynniki.size()-1] + " ");

		wspolczynniki = model->getA2();
		a2->Text = "";
		for(int i=0; i<wspolczynniki.size()-1; i++)
			a2->AppendText(wspolczynniki[i] + " ");
		a2->AppendText(wspolczynniki[wspolczynniki.size()-1] + " ");

		wspolczynniki = model->getB2();
		b2->Text = "";
		for(int i=0; i<wspolczynniki.size()-1; i++)
			b2->AppendText(wspolczynniki[i] + " ");
		b2->AppendText(wspolczynniki[wspolczynniki.size()-1] + " ");

		wspolczynniki = model->getC2();
		c2->Text = "";
		for(int i=0; i<wspolczynniki.size()-1; i++)
			c2->AppendText(wspolczynniki[i] + " ");
		c2->AppendText(wspolczynniki[wspolczynniki.size()-1] + " ");

		d->Text = "" + model->getD();
		i1->Text = "" + model->getI1();
		i2->Text = "" + model->getI2();
		wyMin->Text = "" + model->getWyMin();
		wyMax->Text = "" + model->getWyMax();
		wariancja->Text = "" + model->getWariancja();
		dlZakl->Text = "" + model->getDlZakl();
	}

	private: System::Void zmien_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if( ((Button^)sender)->Name == "zmienA1"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(a1->Text);

			if( !model->zmienParametr("A1", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru A1", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienB1"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(b1->Text);

			if( !model->zmienParametr("B1", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru B1", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienC1"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(c1->Text);

			if( !model->zmienParametr("C1", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru C1", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienA2"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(a2->Text);

			if( !model->zmienParametr("A2", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru A2", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienB2"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(b2->Text);

			if( !model->zmienParametr("B2", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru B2", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienC2"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(c2->Text);

			if( !model->zmienParametr("C2", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru C2", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienWyMin"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMin->Text);

			if( !model->zmienParametr("wyMin", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru wyMin", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienWyMax"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMax->Text);

			if( !model->zmienParametr("wyMax", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru wyMax", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienWariancja"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wariancja->Text);

			if( !model->zmienParametr("WARIANCJA", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru wariancja", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienD"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(d->Text);

			if( !model->zmienParametr("d", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru d", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienI1"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(i1->Text);

			if( !model->zmienParametr("I1", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru I1", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienI2"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(i2->Text);

			if( !model->zmienParametr("I2", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru I2", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		if( ((Button^)sender)->Name == "zmienDlZakl"
			|| ((Button^)sender)->Name == "zmienWszystko")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(dlZakl->Text);

			if( !model->zmienParametr("dlZakl", string(cStr) ))
				MessageBox::Show("Nie uda³o siê zmieniæ parametru d³ugoœæ zak³ócenia", "B³¹d",
					MessageBoxButtons::OK, MessageBoxIcon::Error);

			if(!(((Button^)sender)->Name == "zmienWszystko"))
			{
				delete ident;
				ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
				return;
			}
		}

		delete ident;
		ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), alfa, 1000);
	}

	private: System::Void wczytaj_Click(System::Object^  sender, System::EventArgs^  e)
	{
		double aalfaa;
		if(model != NULL)
		{
			delete model;
			delete ident;
		}

		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nazwaObiektu->Text);

		try
		{
			model = new armax(string(cStr));
			wyswietlParametryObiektu();

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAlfa->Text);
			aalfaa = strtod(cStr, NULL);
			ident = new Identyfikacja(model->get_dA(), model->get_dB(), model->get_dC(), model->getD(), aalfaa, 1000);
			petla->zmienObjekt(model);
		}
		catch(OpenException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
			model = NULL;
		}
		catch(ReadDataException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
			model = NULL;
		}
		catch(...)
		{
			System::Console::WriteLine("Nieznany b³¹d!");
			MessageBox::Show("Nieznany b³¹d!", "B³¹d", MessageBoxButtons::OK);
			exit(1);
		}
	}

// Regulator P

	private: System::Void wyswietlParametryP()
	{
		nazwaRegP->Text = gcnew System::String(reg->getNazwa().c_str());

		k->Text = "" + reg->getK();
		wyMaxP->Text = "" + reg->getWyMax();
		wyMinP->Text = "" + reg->getWyMin();

		nowySkokP->Text = "" + reg->getSkok();
		nowaAmplitudaPP->Text = "" + reg->getProstokatAmplituda();
		nowyOkresPP->Text = "" + reg->getProstokatOkres();
		noweWypelnieniePP->Text = "" + reg->getProstokatWypelnienie();
		nowaAmplitudaSP->Text = "" + reg->getSinusAmplituda();
		nowyOkresSP->Text = "" + reg->getSinusOkres();
		nowaAmplitudaTP->Text = "" + reg->getTrojkatAmplituda();
		nowyOkresTP->Text = "" + reg->getTrojkatOkres();
		nowaWariancjaP->Text = "" + reg->getSzumWariancja();
		nowaLiczbaProbekP->Text = "" + reg->getSzumDlZakl();

		int sygnal = reg->getSygnal();

		if(sygnal & IRegulator::SKOK)
			skokP->Checked = true;
		else
			skokP->Checked = false;

		if(sygnal & IRegulator::KRONEKER)
			kronekerP->Checked = true;
		else
			kronekerP->Checked = false;

		if(sygnal & IRegulator::PROSTOKAT)
			prostokatP->Checked = true;
		else
			prostokatP->Checked = false;

		if(sygnal & IRegulator::SINUS)
			sinusP->Checked = true;
		else
			sinusP->Checked = false;

		if(sygnal & IRegulator::TROJKAT)
			trojkatP->Checked = true;
		else
			trojkatP->Checked = false;

		if(sygnal & IRegulator::SZUM)
			szumP->Checked = true;
		else
			szumP->Checked = false;
	}

	private: System::Void zmienP_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(k->Text);

		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);

		vector<double> param;
		param.push_back( nowaWartosc );
		reg->setNastawyRegulatora(param);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::k, nowaWartosc );
	}
	private: System::Void wczytajRegP_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(reg != NULL)
		{
			delete reg;
		}

		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nazwaRegP->Text);

		try
		{
			reg = new RegulatorP("regulatoryP.Conf", string(cStr));
			wyswietlParametryP();
		}
		catch(OpenException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
			reg = NULL;
		}
		catch(ReadDataException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
			reg = NULL;
		}
		catch(...)
		{
			System::Console::WriteLine("Nieznany b³¹d!");
			MessageBox::Show("Nieznany b³¹d!", "B³¹d", MessageBoxButtons::OK);
			exit(1);
		}
	}

	private: System::Void zmienWymuszenieP_Click(System::Object^  sender, System::EventArgs^  e)
	{
		int nowySygnal = 0;

		if(skokP->Checked)
			nowySygnal += 1;
		if(kronekerP->Checked)
			nowySygnal += 1<<1;
		if(prostokatP->Checked)
			nowySygnal += 1<<2;
		if(sinusP->Checked)
			nowySygnal += 1<<3;
		if(trojkatP->Checked)
			nowySygnal += 1<<4;
		if(szumP->Checked)
			nowySygnal += 1<<5;

		reg->setSygnal(nowySygnal);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::skokP, nowySygnal );

		if(nowySkokP->Text != "")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowySkokP->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			double nowaWartosc = strtod(Liczba.c_str(), NULL);
			
			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowySkokP, nowaWartosc );

			reg->setSkok( nowaWartosc );
		}

		if(nowaAmplitudaPP->Text != "" && nowyOkresPP->Text != "" && noweWypelnieniePP->Text != "")
		{
			double aml, okr, wyp;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaPP->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);
			
			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaPP, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresPP->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresPP, okr );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(noweWypelnieniePP->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			wyp = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::noweWypelnieniePP, wyp );
			
			reg->setProstokat(aml,okr,wyp);
		}

		if(nowaAmplitudaSP->Text != "" && nowyOkresSP->Text != "")
		{
			double aml, okr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaSP->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaSP, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresSP->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresSP, okr );

			reg->setSinus(aml,okr);
		}

		if(nowaAmplitudaTP->Text != "" && nowyOkresTP->Text != "")
		{
			double aml, okr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaTP->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaTP, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresTP->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresTP, okr );

			reg->setTrojkat(aml,okr);
		}

		if(nowaWariancjaP->Text != "" && nowaLiczbaProbekP->Text != "")
		{
			double war, lpr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaWariancjaP->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			war = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaWariancjaP, war );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaLiczbaProbekP->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			lpr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaLiczbaProbekP, lpr );

			reg->setSzum(war,lpr);
		}
	}

	private: System::Void zmienWyMaxP_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMaxP->Text);
		
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);

		reg->setWyMax(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMaxP, nowaWartosc );
	}

	private: System::Void zmienWyMinP_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMinP->Text);

		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);

		reg->setWyMin(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMinP, nowaWartosc );
	}

	private: System::Void zmienWszystkoP_Click(System::Object^  sender, System::EventArgs^  e)
	{
		vector<double> param;

		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(k->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );
		reg->setNastawyRegulatora(param);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::k, strtod(cStr, NULL) );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMaxP->Text);

		Liczba = cStr;
		    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);
		reg->setWyMax(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMaxP, strtod(cStr, NULL) );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMinP->Text);

		Liczba = cStr;
		    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);
		reg->setWyMin(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMinP, strtod(cStr, NULL) );
	}

	private: System::Void zapiszP_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* nazwa = (char*)(void*)Marshal::StringToHGlobalAnsi(nazwaReg_P->Text);
		char* plik = (char*)(void*)Marshal::StringToHGlobalAnsi(nazwaPlikuP->Text);

		try
		{
			reg->zapisNastawRegulatora(plik, nazwa);
		}
		catch(OpenException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
		}
	}

// Regulator PID

	private: System::Void wyswietlParametryPID()
	{
		nazwaRegPID->Text = gcnew System::String(regPID->getNazwa().c_str());

		kPID->Text = "" + regPID->getK();
		tiPID->Text = "" + regPID->getTi();
		tdPID->Text = "" + regPID->getTd();
		tpPID->Text = "" + regPID->getTp();
		wpPID->Text = "" + regPID->getWarPocz();
		nPID->Text = "" + regPID->getN();
		bPID->Text = "" + regPID->getB();
		wyMaxPID->Text = "" + regPID->getWyMax();
		wyMinPID->Text = "" + regPID->getWyMin();

		nowySkokPID->Text = "" + regPID->getSkok();
		nowaAmplitudaPPID->Text = "" + regPID->getProstokatAmplituda();
		nowyOkresPPID->Text = "" + regPID->getProstokatOkres();
		noweWypelnieniePPID->Text = "" + regPID->getProstokatWypelnienie();
		nowaAmplitudaSPID->Text = "" + regPID->getSinusAmplituda();
		nowyOkresSPID->Text = "" + regPID->getSinusOkres();
		nowaAmplitudaTPID->Text = "" + regPID->getTrojkatAmplituda();
		nowyOkresTPID->Text = "" + regPID->getTrojkatOkres();
		nowaWariancjaPID->Text = "" + regPID->getSzumWariancja();
		nowaLiczbaProbekPID->Text = "" + regPID->getSzumDlZakl();

		int sygnal = regPID->getSygnal();

		if(sygnal & IRegulator::SKOK)
			skokPID->Checked = true;
		else
			skokPID->Checked = false;

		if(sygnal & IRegulator::KRONEKER)
			kronekerPID->Checked = true;
		else
			kronekerPID->Checked = false;

		if(sygnal & IRegulator::PROSTOKAT)
			prostokatPID->Checked = true;
		else
			prostokatPID->Checked = false;

		if(sygnal & IRegulator::SINUS)
			sinusPID->Checked = true;
		else
			sinusPID->Checked = false;

		if(sygnal & IRegulator::TROJKAT)
			trojkatPID->Checked = true;
		else
			trojkatPID->Checked = false;

		if(sygnal & IRegulator::SZUM)
			szumPID->Checked = true;
		else
			szumPID->Checked = false;
	}

	private: System::Void wczytajRegPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(reg != NULL)
		{
			delete regPID;
		}

		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nazwaRegPID->Text);

		try
		{
			regPID = new RegulatorPID("regulatoryPID.Conf", string(cStr));
			wyswietlParametryPID();
		}
		catch(OpenException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
			regPID = NULL;
		}
		catch(ReadDataException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
			regPID = NULL;
		}
		catch(...)
		{
			System::Console::WriteLine("Nieznany b³¹d!");
			MessageBox::Show("Nieznany b³¹d!", "B³¹d", MessageBoxButtons::OK);
			exit(1);
		}
	}

	private: System::Void zmienWymuszeniePID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		int nowySygnal = 0;

		if(skokPID->Checked)
			nowySygnal += 1;
		if(kronekerPID->Checked)
			nowySygnal += 1<<1;
		if(prostokatPID->Checked)
			nowySygnal += 1<<2;
		if(sinusPID->Checked)
			nowySygnal += 1<<3;
		if(trojkatPID->Checked)
			nowySygnal += 1<<4;
		if(szumPID->Checked)
			nowySygnal += 1<<5;

		regPID->setSygnal(nowySygnal);

	    for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::skokPID, nowySygnal );

		if(nowySkokPID->Text != "")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowySkokPID->Text);

			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			double nowaWartosc = strtod(Liczba.c_str(), NULL);
			
			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowySkokPID, nowaWartosc );

			regPID->setSkok( nowaWartosc );
		}

		if(nowaAmplitudaPPID->Text != "" && nowyOkresPPID->Text != "" && noweWypelnieniePPID->Text != "")
		{
			double aml, okr, wyp;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaPPID->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaPPID, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresPPID->Text);

			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresPPID, okr );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(noweWypelnieniePPID->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			wyp = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::noweWypelnieniePPID, wyp );

			regPID->setProstokat(aml,okr,wyp);
		}

		if(nowaAmplitudaSPID->Text != "" && nowyOkresSPID->Text != "")
		{
			double aml, okr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaSPID->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaSPID, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresSPID->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresSPID, okr );

			regPID->setSinus(aml,okr);
		}

		if(nowaAmplitudaTPID->Text != "" && nowyOkresTPID->Text != "")
		{
			double aml, okr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaTPID->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaTPID, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresTPID->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresTPID, okr );

			regPID->setTrojkat(aml,okr);
		}

		if(nowaWariancjaPID->Text != "" && nowaLiczbaProbekPID->Text != "")
		{
			double war, lpr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaWariancjaPID->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			war = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaWariancjaPID, war );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaLiczbaProbekPID->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			lpr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaLiczbaProbekPID, lpr );

			regPID->setSzum(war,lpr);
		}
	}

	private: System::Void zapiszPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* nazwa = (char*)(void*)Marshal::StringToHGlobalAnsi(nazwaReg_PID->Text);
		char* plik = (char*)(void*)Marshal::StringToHGlobalAnsi(nazwaPlikuPID->Text);

		try
		{
			regPID->zapisNastawRegulatora(plik, nazwa);
		}
		catch(OpenException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
		}
	}

	private: System::Void zmienKPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(kPID->Text);
		
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);

		regPID->setNastawyRegulatora(RegulatorPID::K, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::kPID, nowaWartosc );
	}

	private: System::Void zmienTiPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(tiPID->Text);

		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);

		regPID->setNastawyRegulatora(RegulatorPID::TI, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::tiPID, nowaWartosc );
	}

	private: System::Void zmienTdPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(tdPID->Text);

		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);

		regPID->setNastawyRegulatora(RegulatorPID::TD, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::tdPID, nowaWartosc );
	}

	private: System::Void zmienTpPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(tpPID->Text);

		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);

		regPID->setNastawyRegulatora(RegulatorPID::TP, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::tpPID, nowaWartosc );
	}

	private: System::Void zmienWpPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wpPID->Text);

		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);

		regPID->setNastawyRegulatora(RegulatorPID::WARUNEK_POCZATKOWY, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wpPID, nowaWartosc );
	}

	private: System::Void zmienNPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nPID->Text);
		
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);

		regPID->setNastawyRegulatora(RegulatorPID::N_PARAM, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::nPID, nowaWartosc );
	}

	private: System::Void zmienBPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(bPID->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regPID->setNastawyRegulatora(RegulatorPID::B, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::bPID, nowaWartosc );
	}

	private: System::Void zmienWyMaxPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMaxPID->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regPID->setWyMax(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMaxPID, nowaWartosc );
	}

	private: System::Void zmienWyMinPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMinPID->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regPID->setWyMin(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMinPID, nowaWartosc );
	}

	private: System::Void zmienWszystkoPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		vector<double> param;

		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(kPID->Text);

		string Liczba = cStr;
		size_t Znalezione;

		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		double nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::kPID, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(tiPID->Text);

		Liczba = cStr;
		    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::tiPID, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(tdPID->Text);

		Liczba = cStr;
		    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::tdPID, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(tpPID->Text);

		Liczba = cStr;
		    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::tpPID, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wpPID->Text);

		Liczba = cStr;
		    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wpPID, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nPID->Text);

		Liczba = cStr;
		    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::nPID, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(bPID->Text);

		Liczba = cStr;
		    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::bPID, nowaWartosc );

		regPID->setNastawyRegulatora(param);
		
		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMaxPID->Text);

		Liczba = cStr;
		    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		regPID->setWyMax(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMaxPID, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMinPID->Text);

		Liczba = cStr;
		    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		regPID->setWyMin(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMinPID, nowaWartosc );
	}

// Regulator Adaptacyjny PID


	private: System::Void wyswietlParametryAPID()
	{
		kAPID->Text = "" + regAPID->getK();
		tiAPID->Text = "" + regAPID->getTi();
		tdAPID->Text = "" + regAPID->getTd();
		onAPID->Text = "" + regAPID->getOn();
		wyMaxAPID->Text = "" + regAPID->getWyMax();
		wyMinAPID->Text = "" + regAPID->getWyMin();

		nowySkokAPID->Text = "" + regAPID->getSkok();
		nowaAmplitudaPAPID->Text = "" + regAPID->getProstokatAmplituda();
		nowyOkresPAPID->Text = "" + regAPID->getProstokatOkres();
		noweWypelnieniePAPID->Text = "" + regAPID->getProstokatWypelnienie();
		nowaAmplitudaSAPID->Text = "" + regAPID->getSinusAmplituda();
		nowyOkresSAPID->Text = "" + regAPID->getSinusOkres();
		nowaAmplitudaTAPID->Text = "" + regAPID->getTrojkatAmplituda();
		nowyOkresTAPID->Text = "" + regAPID->getTrojkatOkres();
		nowaWariancjaAPID->Text = "" + regAPID->getSzumWariancja();
		nowaLiczbaProbekAPID->Text = "" + regAPID->getSzumDlZakl();

		int sygnal = regAPID->getSygnal();

		if(sygnal & IRegulator::SKOK)
			skokAPID->Checked = true;
		else
			skokAPID->Checked = false;

		if(sygnal & IRegulator::KRONEKER)
			kronekerAPID->Checked = true;
		else
			kronekerAPID->Checked = false;

		if(sygnal & IRegulator::PROSTOKAT)
			prostokatAPID->Checked = true;
		else
			prostokatAPID->Checked = false;

		if(sygnal & IRegulator::SINUS)
			sinusAPID->Checked = true;
		else
			sinusAPID->Checked = false;

		if(sygnal & IRegulator::TROJKAT)
			trojkatAPID->Checked = true;
		else
			trojkatAPID->Checked = false;

		if(sygnal & IRegulator::SZUM)
			szumAPID->Checked = true;
		else
			szumAPID->Checked = false;
	}

	private: System::Void zmienWymuszenieAPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		int nowySygnal = 0;

		if(skokAPID->Checked)
			nowySygnal += 1;
		if(kronekerAPID->Checked)
			nowySygnal += 1<<1;
		if(prostokatAPID->Checked)
			nowySygnal += 1<<2;
		if(sinusAPID->Checked)
			nowySygnal += 1<<3;
		if(trojkatAPID->Checked)
			nowySygnal += 1<<4;
		if(szumAPID->Checked)
			nowySygnal += 1<<5;

		regAPID->setSygnal(nowySygnal);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::skokAPID, nowySygnal );

		if(nowySkokAPID->Text != "")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowySkokAPID->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			double nowaWartosc = strtod(Liczba.c_str(), NULL);
			
			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowySkokAPID, nowaWartosc );
			
			regAPID->setSkok( strtod(cStr, NULL) );
		}

		if(nowaAmplitudaPAPID->Text != "" && nowyOkresPAPID->Text != "" && noweWypelnieniePAPID->Text != "")
		{
			double aml, okr, wyp;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaPAPID->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaPAPID, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresPAPID->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresPAPID, okr );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(noweWypelnieniePAPID->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			wyp = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::noweWypelnieniePAPID, wyp );

			regAPID->setProstokat(aml,okr,wyp);
		}

		if(nowaAmplitudaSAPID->Text != "" && nowyOkresSAPID->Text != "")
		{
			double aml, okr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaSAPID->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaSAPID, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresSAPID->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresSAPID, okr );

			regAPID->setSinus(aml,okr);
		}

		if(nowaAmplitudaTAPID->Text != "" && nowyOkresTAPID->Text != "")
		{
			double aml, okr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaTAPID->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaTAPID, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresTAPID->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresTAPID, okr );

			regAPID->setTrojkat(aml,okr);
		}

		if(nowaWariancjaAPID->Text != "" && nowaLiczbaProbekAPID->Text != "")
		{
			double war, lpr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaWariancjaAPID->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			war = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaWariancjaAPID, war );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaLiczbaProbekAPID->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			lpr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaLiczbaProbekAPID, lpr );

			regAPID->setSzum(war,lpr);
		}
	}

	private: System::Void zmienKAPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(kAPID->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regAPID->setNastawyRegulatora(AdaptPID::K, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::kAPID, nowaWartosc );
	}

	private: System::Void zmienTiAPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(tiAPID->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regAPID->setNastawyRegulatora(AdaptPID::TI, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::tiAPID, nowaWartosc );
	}

	private: System::Void zmienTdAPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(tdAPID->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regAPID->setNastawyRegulatora(AdaptPID::TD, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::tdAPID, nowaWartosc );
	}

	private: System::Void zmienOnAPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(onAPID->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regAPID->setNastawyRegulatora(AdaptPID::ON, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::onAPID, nowaWartosc );
	}

	private: System::Void zmienWyMaxAPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMaxAPID->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regAPID->setWyMax(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMaxAPID, nowaWartosc );
	}

	private: System::Void zmienWyMinAPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMinAPID->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regAPID->setWyMin(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMinAPID, nowaWartosc );
	}

	private: System::Void zmienWszystkoAPID_Click(System::Object^  sender, System::EventArgs^  e)
	{
		vector<double> param;

		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(kAPID->Text);
		

		string Liczba = cStr;
		size_t Znalezione;

		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::kAPID, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(tiAPID->Text);

		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::tiAPID, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(tdAPID->Text);

		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		param.push_back( strtod(cStr, NULL) );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::tdAPID, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(onAPID->Text);

		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		param.push_back( strtod(cStr, NULL) );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::onAPID, nowaWartosc );

		regAPID->setNastawyRegulatora(param);

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMaxAPID->Text);
		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );
		regAPID->setWyMax(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMaxAPID, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMinAPID->Text);
		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );
		regAPID->setWyMin(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMinAPID, nowaWartosc );
	}

	private:delegate System::Void wyswietlParametryAPID_d();
	private:delegate System::Void wlaczKontrolkiAPID_d();
	private:delegate System::Void wylaczKontrolkiAPID_d();
	

	private: System::Void ekspAPID_Changed(System::Object^  sender, System::EventArgs^  e) 
	{
		wyswietlParametryAPID_d^ wysPar = gcnew wyswietlParametryAPID_d(this, &PSS_LAB::Form1::wyswietlParametryAPID);
		wlaczKontrolkiAPID_d^ wlaczKontr = gcnew wlaczKontrolkiAPID_d(this, &PSS_LAB::Form1::wlaczKontrolkiAPID);
		wylaczKontrolkiAPID_d^ wylaczKontr = gcnew wylaczKontrolkiAPID_d(this, &PSS_LAB::Form1::wylaczKontrolkiAPID);

		if( (isAPID) && (ZmienNastawy->Checked) && ZmienNastawy->Enabled)
		{
			this->Invoke(wylaczKontr);

			regAPID->wlaczStrojenie();

			for(int i=0; i<iloscKlientow; i++)
				WyslijDoKlienta(dyskryptory[i], TYP::ZmienNastawy, 1);

		}

		if( (ZmienNastawy->Checked)&& ZmienNastawy->Enabled )
		{
			for(int i=0; i<iloscKlientow; i++)
				WyslijDoKlienta(dyskryptory[i], TYP::ZmienNastawy, 1);
		}

		if( (!ZmienNastawy->Checked)&& ZmienNastawy->Enabled )
		{
			for(int i=0; i<iloscKlientow; i++)
				WyslijDoKlienta(dyskryptory[i], TYP::ZmienNastawy, 2);
		}
		
		if( (isAPID) && (!ZmienNastawy->Enabled) && (!regAPID->isStrojenie()))
		{
			this->Invoke(wysPar);
			this->Invoke(wlaczKontr);

			for(int i=0; i<iloscKlientow; i++)
				WyslijDoKlienta(dyskryptory[i], TYP::ZmienNastawy, 0);
		}
	}

	private: System::Void wylaczKontrolkiAPID()
	{
		typRegulatora->Enabled = false;
		ZmienNastawy->Enabled = false;
		kAPID->Enabled = false;
		tiAPID->Enabled = false;
		tdAPID->Enabled = false;
		onAPID->Enabled = false;
		wyMaxAPID->Enabled = false;
		wyMinAPID->Enabled = false;
	}

	private: System::Void wlaczKontrolkiAPID()
	{
		typRegulatora->Enabled = true;
		ZmienNastawy->Enabled = true;
		ZmienNastawy->Checked = false;
		kAPID->Enabled = true;
		tiAPID->Enabled = true;
		tdAPID->Enabled = true;
		onAPID->Enabled = true;
		wyMaxAPID->Enabled = true;
		wyMinAPID->Enabled = true;
	}

	



// R Ó ¯ N E

	private: System::Void czyscIdent_Click(System::Object^  sender, System::EventArgs^  e)
	{
		identyfikacja_textBox->Clear();
	}

	private: System::Void zmienAlfa_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAlfa->Text);
		alfa = strtod(cStr, NULL);
		ident->setAlfa(alfa);
	}

    // G P C

	private: System::Void wyswietlParametryGPC()
	{
		nazwaRegGPC->Text = gcnew System::String(regGPC->getNazwa().c_str()); 

		HGPC->Text = "" + regGPC->getH();
		LGPC->Text = "" + regGPC->getL();
		alfaGPC->Text = "" + regGPC->getAlfa();
		roGPC->Text = "" + regGPC->getRo();
		dAGPC->Text = "" + regGPC->getDA();
		dBGPC->Text = "" + regGPC->getDB();
		wyMaxGPC->Text = "" + regGPC->getWyMax();
		wyMinGPC->Text = "" + regGPC->getWyMin();

		nowySkokGPC->Text = "" + regGPC->getSkok();
		nowaAmplitudaPGPC->Text = "" + regGPC->getProstokatAmplituda();
		nowyOkresPGPC->Text = "" + regGPC->getProstokatOkres();
		noweWypelnieniePGPC->Text = "" + regGPC->getProstokatWypelnienie();
		nowaAmplitudaSGPC->Text = "" + regGPC->getSinusAmplituda();
		nowyOkresSGPC->Text = "" + regGPC->getSinusOkres();
		nowaAmplitudaTGPC->Text = "" + regGPC->getTrojkatAmplituda();
		nowyOkresTGPC->Text = "" + regGPC->getTrojkatOkres();
		nowaWariancjaGPC->Text = "" + regGPC->getSzumWariancja();
		nowaLiczbaProbekGPC->Text = "" + regGPC->getSzumDlZakl();

		int sygnal = regGPC->getSygnal();

		if(sygnal & IRegulator::SKOK)
			skokGPC->Checked = true;
		else
			skokGPC->Checked = false;

		if(sygnal & IRegulator::KRONEKER)
			kronekerGPC->Checked = true;
		else
			kronekerGPC->Checked = false;

		if(sygnal & IRegulator::PROSTOKAT)
			prostokatGPC->Checked = true;
		else
			prostokatGPC->Checked = false;

		if(sygnal & IRegulator::SINUS)
			sinusGPC->Checked = true;
		else
			sinusGPC->Checked = false;

		if(sygnal & IRegulator::TROJKAT)
			trojkatGPC->Checked = true;
		else
			trojkatGPC->Checked = false;

		if(sygnal & IRegulator::SZUM)
			szumGPC->Checked = true;
		else
			szumGPC->Checked = false;
	}


	private: System::Void wczytajRegGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(reg != NULL)
		{
			delete regGPC;
		}

		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nazwaRegGPC->Text);

		try
		{
			regGPC = new GPC("regulatorGPC.conf", string(cStr),1);
			wyswietlParametryGPC();
		}
		catch(OpenException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
			regGPC = NULL;
		}
		catch(ReadDataException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
			regGPC = NULL;
		}
		catch(...)
		{
			System::Console::WriteLine("Nieznany b³¹d!");
			MessageBox::Show("Nieznany b³¹d!", "B³¹d", MessageBoxButtons::OK);
			exit(1);
		}
	}

	private: System::Void zmienWymuszenieGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		int nowySygnal = 0;

		if(skokGPC->Checked)
			nowySygnal += 1;
		if(kronekerGPC->Checked)
			nowySygnal += 1<<1;
		if(prostokatGPC->Checked)
			nowySygnal += 1<<2;
		if(sinusGPC->Checked)
			nowySygnal += 1<<3;
		if(trojkatGPC->Checked)
			nowySygnal += 1<<4;
		if(szumGPC->Checked)
			nowySygnal += 1<<5;

		regGPC->setSygnal(nowySygnal);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::skokGPC, nowySygnal );

		if(nowySkokGPC->Text != "")
		{
			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowySkokGPC->Text);

			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			double nowaWartosc = strtod(Liczba.c_str(), NULL);

			regGPC->setSkok( nowaWartosc );

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowySkokGPC, nowaWartosc );
		}

		if(nowaAmplitudaPGPC->Text != "" && nowyOkresPGPC->Text != "" && noweWypelnieniePGPC->Text != "")
		{
			double aml, okr, wyp;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaPGPC->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaPGPC, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresPGPC->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresPGPC, okr );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(noweWypelnieniePGPC->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			wyp = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::noweWypelnieniePGPC, wyp );

			regGPC->setProstokat(aml,okr,wyp);
		}

		if(nowaAmplitudaSGPC->Text != "" && nowyOkresSGPC->Text != "")
		{
			double aml, okr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaSGPC->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaSGPC, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresSGPC->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresSGPC, okr );

			regGPC->setSinus(aml,okr);
		}

		if(nowaAmplitudaTGPC->Text != "" && nowyOkresTGPC->Text != "")
		{
			double aml, okr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAmplitudaTGPC->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			aml = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAmplitudaTGPC, aml );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowyOkresTGPC->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			okr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowyOkresTGPC, okr );

			regGPC->setTrojkat(aml,okr);
		}

		if(nowaWariancjaGPC->Text != "" && nowaLiczbaProbekGPC->Text != "")
		{
			double war, lpr;

			char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaWariancjaGPC->Text);
			
			string Liczba = cStr;
			size_t Znalezione;

			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			war = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaWariancjaGPC, war );

			cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaLiczbaProbekGPC->Text);
			
			Liczba = cStr;
			
			Znalezione = Liczba.find( ',' );
			if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

			lpr = strtod(Liczba.c_str(), NULL);

			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::nowaLiczbaProbekGPC, lpr );

			regGPC->setSzum(war,lpr);
		}
	}

	private: System::Void zapiszGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* nazwa = (char*)(void*)Marshal::StringToHGlobalAnsi(nazwaReg_GPC->Text);
		char* plik = (char*)(void*)Marshal::StringToHGlobalAnsi(nazwaPlikuGPC->Text);

		try
		{
			regGPC->zapisNastawRegulatora(plik, nazwa);
		}
		catch(OpenException& exc)
		{
			System::Console::WriteLine(exc.what());
			MessageBox::Show(gcnew System::String(exc.what()), "B³¹d", MessageBoxButtons::OK);
		}
	}

	private: System::Void zmienHGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(HGPC->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regGPC->setNastawyRegulatora(GPC::pH, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::HGPC, nowaWartosc );
	}

	private: System::Void zmienLGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(LGPC->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regGPC->setNastawyRegulatora(GPC::pL, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::LGPC, nowaWartosc );
	}

	private: System::Void zmienroGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(roGPC->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regGPC->setNastawyRegulatora(GPC::pro, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::roGPC, nowaWartosc );
	}

	private: System::Void zmienalfaGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(alfaGPC->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regGPC->setNastawyRegulatora(GPC::palfa, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::alfaGPC, nowaWartosc );
	}
     
    private: System::Void zmiendAGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(dAGPC->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regGPC->setNastawyRegulatora(GPC::pdA, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::dAGPC, nowaWartosc );

		double aalfaa2;
		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAlfaGPC->Text);
		aalfaa2 = strtod(cStr, NULL);
		regGPC->setParamABK(nowaWartosc,regGPC->getDB(),aalfaa2);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAlfaGPC, strtod(cStr, NULL) );
	}

	private: System::Void zmiendBGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(dBGPC->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regGPC->setNastawyRegulatora(GPC::pdB, nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::dBGPC, nowaWartosc );

		double aalfaa2;
		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAlfaGPC->Text);
		aalfaa2 = strtod(cStr, NULL);
		regGPC->setParamABK(regGPC->getDA(),nowaWartosc,aalfaa2);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAlfaGPC, strtod(cStr, NULL) );
	}

	private: System::Void zmienWyMaxGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMaxGPC->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regGPC->setWyMax(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMaxGPC, nowaWartosc );
	}

	private: System::Void zmienWyMinGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMinGPC->Text);
		string Liczba = cStr;
		size_t Znalezione;
    
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';
		
		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		regGPC->setWyMin(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMinGPC, nowaWartosc );
	}

	private: System::Void zmienWszystkoGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		vector<double> param;

		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(HGPC->Text);
		

		string Liczba = cStr;
		size_t Znalezione;

		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		double nowaWartosc = strtod(Liczba.c_str(), NULL);
		param.push_back( nowaWartosc );
		
		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::HGPC, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(LGPC->Text);

		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::LGPC, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(roGPC->Text);

		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::roGPC, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(alfaGPC->Text);

		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::alfaGPC, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(dAGPC->Text);

		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::dAGPC, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(dBGPC->Text);

		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);

		param.push_back( nowaWartosc );

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::dBGPC, nowaWartosc );

		regGPC->setNastawyRegulatora(param);

		double aalfaa2;
		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAlfaGPC->Text);
		aalfaa2 = strtod(cStr, NULL);
		regGPC->setParamABK(regGPC->getDA(),regGPC->getDB(),aalfaa2);


		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMaxGPC->Text);
		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);
		regGPC->setWyMax(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMaxGPC, nowaWartosc );

		cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(wyMinGPC->Text);
		Liczba = cStr;
		
		Znalezione = Liczba.find( ',' );
		if( Znalezione != string::npos ) Liczba[Znalezione] = '.';

		nowaWartosc = strtod(Liczba.c_str(), NULL);
		regGPC->setWyMin(nowaWartosc);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::wyMinGPC, nowaWartosc );
	}

	private: System::Void zmiennAlfaGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		char* cStr = (char*)(void*)Marshal::StringToHGlobalAnsi(nowaAlfaGPC->Text);
		alfaaGPC = strtod(cStr, NULL);
		regGPC->identyf->setAlfa(alfaaGPC);

		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::nowaAlfaGPC, strtod(cStr, NULL) );
	}

	private: System::Void czyscIdentGPC_Click(System::Object^  sender, System::EventArgs^  e)
	{
		identyfikacjaGPC_textBox->Clear();
	}

	private: System::Void typRegulatora_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		for(int i=0; i<iloscKlientow; i++)
			this->WyslijDoKlienta(dyskryptory[i], TYP::typRegulatora, typRegulatora->SelectedIndex);

		if(typRegulatora->SelectedIndex == 0) 
		{
			petla = new PetlaRegulacji(model, reg);
			regNieGPC = true;
			isAPID = false;
			identyfikacjaGPC_textBox->Clear();
			regGPC->identyf->reset();
		}
		else if(typRegulatora->SelectedIndex == 1)
		{
			petla = new PetlaRegulacji(model, regPID);
			regNieGPC = true;
			isAPID = false;
			identyfikacjaGPC_textBox->Clear();
			regGPC->identyf->reset();
		}
		else if(typRegulatora->SelectedIndex == 2)
		{
			petla = new PetlaRegulacji(model, regGPC);
			regNieGPC = false;
			isAPID = false;
			identyfikacjaGPC_textBox->Clear();
			krokGPC = 0;
			
		}
		else if(typRegulatora->SelectedIndex == 3)
		{
			petla = new PetlaRegulacji(model, regAPID);
			regNieGPC = true;
			isAPID = true;
			identyfikacjaGPC_textBox->Clear();
			regGPC->identyf->reset();
		}
		 
   }

	private: System::Void stop_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if (stop->Checked)
			for(int i=0; i<iloscKlientow; i++)
				this->WyslijDoKlienta(dyskryptory[i], TYP::stop, 1);
	}
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
};	

}