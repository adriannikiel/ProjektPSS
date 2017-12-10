#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PSS_LAB {

	/// <summary>
	/// Summary for Wykres
	/// </summary>
	public ref class Wykres : public System::Windows::Forms::UserControl
	{
	public:
		Wykres(double xMin, double xMax, double yMin, double yMax,
			double kwant, bool sitka, double poczatek)
		{
			InitializeComponent();

			this->xMin = xMin;
			this->xMax = xMax;
			this->yMin = yMin;
			this->yMax = yMax;
			this->kwant = kwant;
			this->siatka = sitka;
			this->poczatek = poczatek;
		}

		Wykres(void)
		{
			InitializeComponent();

			xMin = 0;
			xMax = 100;
			yMin = -50;
			yMax = 50;
			siatka = true;
			kwant = 1;
			poczatek = 0;
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Wykres()
		{
			if (components)
			{
				delete components;
			}
		}
	
	public:
		void odswierz()
		{
			Graphics^ g = obraz->CreateGraphics();
			g->Clear(System::Drawing::Color::White);

			Pen^ pioro = gcnew Pen(System::Drawing::Color::Black);
			SolidBrush^ pedzelString = gcnew SolidBrush(System::Drawing::Color::Black);
			System::Drawing::Font^ czcionka = gcnew System::Drawing::Font(System::Drawing::FontFamily::GenericSansSerif, 6, FontStyle::Regular);

			pioro->EndCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;
			// rysowanie osi X
			g->DrawLine(pioro, 30, obraz->Height-20, obraz->Width-30, obraz->Height-20);
			pioro->EndCap = System::Drawing::Drawing2D::LineCap::NoAnchor;
			for(int i=0; i<=10; i++)
			{
				float jednostka = (obraz->Width-70)/10;
				g->DrawLine(pioro, 30+i*jednostka, obraz->Height-20.0F, 30+i*jednostka, obraz->Height-16.0F);
				g->DrawString( (xMin + i*(xMax-xMin)/10).ToString(), czcionka, pedzelString, 25+i*jednostka, obraz->Height-15);
				if(siatka && i!=0)
				{
					pioro->Color = System::Drawing::Color::LightGray;
					g->DrawLine(pioro, 30+i*jednostka, obraz->Height-20.0F, 30+i*jednostka, 10.0F);
					pioro->Color = System::Drawing::Color::Black;
				}
			}

			pioro->EndCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;
			// rysowanie osi Y
			g->DrawLine(pioro, 30, obraz->Height-20, 30, 10);
			pioro->EndCap = System::Drawing::Drawing2D::LineCap::NoAnchor;
			for(int i=0; i<=10; i++)
			{
				float jednostka = (obraz->Height-40)/10;
				g->DrawLine(pioro, 26.0F, obraz->Height-20-i*jednostka, 30.0F, obraz->Height-20-i*jednostka);
				g->DrawString( (yMin + i*(yMax-yMin)/10).ToString(), czcionka, pedzelString, 5, obraz->Height-25-i*jednostka);
				if(siatka && i!=0)
				{
					pioro->Color = System::Drawing::Color::LightGray;
					g->DrawLine(pioro, 31.0F, obraz->Height-20-i*jednostka, obraz->Width-30.0F, obraz->Height-20-i*jednostka);
					pioro->Color = System::Drawing::Color::Black;
				}
			}

			// rysowanie wykresu
			for(int i=0; i<y.Count-1; i++)
			{
				float jednostkaY = (obraz->Height-40)/(yMax-yMin);
				float zeroY = obraz->Height-20 + yMin*jednostkaY;
				float war1 = zeroY - jednostkaY*(double)y[i];
				float war2 = zeroY - jednostkaY*(double)y[i+1];

				float jednostkaX = (obraz->Width-70)/(xMax-xMin);
				float zeroX = 30 - xMin*jednostkaX;

				float zakres = xMax - xMin;
				pioro->Color = System::Drawing::Color::Green;
				g->DrawLine(pioro, zeroX+(obraz->Width-70)*i*kwant/zakres, war1, zeroX+(obraz->Width-70)*(i+1)*kwant/zakres, war2);
			}

			for(int i=0; i<y1.Count-1; i++)
			{
				float jednostkaY = (obraz->Height-40)/(yMax-yMin);
				float zeroY = obraz->Height-20 + yMin*jednostkaY;
				float war1 = zeroY - jednostkaY*(double)y1[i];
				float war2 = zeroY - jednostkaY*(double)y1[i+1];

				float jednostkaX = (obraz->Width-70)/(xMax-xMin);
				float zeroX = 30 - xMin*jednostkaX;

				float zakres = xMax - xMin;
				pioro->Color = System::Drawing::Color::Red;
				g->DrawLine(pioro, zeroX+(obraz->Width-70)*i*kwant/zakres, war1, zeroX+(obraz->Width-70)*(i+1)*kwant/zakres, war2);
			}
		}

	private: ArrayList y;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  xMi;
	private: System::Windows::Forms::NumericUpDown^  yMa;


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  yMi;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  xMa;

	private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Panel^  panel1;
	private: ArrayList y1;

	public:
		void odswierz(IEnumerable^ osY)
		{
			y1.Clear();

			System::Collections::IEnumerator^ enumerator = osY->GetEnumerator();
			y.Clear();
			while(enumerator->MoveNext())
				y.Add(System::Double::Parse(enumerator->Current->ToString()));

			Graphics^ g = obraz->CreateGraphics();
			g->Clear(System::Drawing::Color::White);

			Pen^ pioro = gcnew Pen(System::Drawing::Color::Black);
			SolidBrush^ pedzelString = gcnew SolidBrush(System::Drawing::Color::Black);
			System::Drawing::Font^ czcionka = gcnew System::Drawing::Font(System::Drawing::FontFamily::GenericSansSerif, 6, FontStyle::Regular);

			pioro->EndCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;
			// rysowanie osi X
			g->DrawLine(pioro, 30, obraz->Height-20, obraz->Width-30, obraz->Height-20);
			pioro->EndCap = System::Drawing::Drawing2D::LineCap::NoAnchor;
			for(int i=0; i<=10; i++)
			{
				float jednostka = (obraz->Width-70)/10;
				g->DrawLine(pioro, 30+i*jednostka, obraz->Height-20.0F, 30+i*jednostka, obraz->Height-16.0F);
				g->DrawString( (xMin + i*(xMax-xMin)/10).ToString(), czcionka, pedzelString, 25+i*jednostka, obraz->Height-15);
				if(siatka && i!=0)
				{
					pioro->Color = System::Drawing::Color::LightGray;
					g->DrawLine(pioro, 30+i*jednostka, obraz->Height-20.0F, 30+i*jednostka, 10.0F);
					pioro->Color = System::Drawing::Color::Black;
				}
			}

			pioro->EndCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;
			// rysowanie osi Y
			g->DrawLine(pioro, 30, obraz->Height-20, 30, 10);
			pioro->EndCap = System::Drawing::Drawing2D::LineCap::NoAnchor;
			for(int i=0; i<=10; i++)
			{
				float jednostka = (obraz->Height-40)/10;
				g->DrawLine(pioro, 26.0F, obraz->Height-20-i*jednostka, 30.0F, obraz->Height-20-i*jednostka);
				g->DrawString( (yMin + i*(yMax-yMin)/10).ToString(), czcionka, pedzelString, 5, obraz->Height-25-i*jednostka);
				if(siatka && i!=0)
				{
					pioro->Color = System::Drawing::Color::LightGray;
					g->DrawLine(pioro, 31.0F, obraz->Height-20-i*jednostka, obraz->Width-30.0F, obraz->Height-20-i*jednostka);
					pioro->Color = System::Drawing::Color::Black;
				}
			}

			// rysowanie wykresu
			for(int i=0; i<y.Count-1; i++)
			{
				float jednostkaY = (obraz->Height-40)/(yMax-yMin);
				float zeroY = obraz->Height-20 + yMin*jednostkaY;
				float war1 = zeroY - jednostkaY*(double)y[i];
				float war2 = zeroY - jednostkaY*(double)y[i+1];

				float jednostkaX = (obraz->Width-70)/(xMax-xMin);
				float zeroX = 30 - xMin*jednostkaX;

				float zakres = xMax - xMin;
				pioro->Color = System::Drawing::Color::Green;
				g->DrawLine(pioro, zeroX+(obraz->Width-70)*i*kwant/zakres, war1, zeroX+(obraz->Width-70)*(i+1)*kwant/zakres, war2);
			}
		}

	public:
		void odswierz(IEnumerable^ osY, IEnumerable^ osY1)
		{
			System::Collections::IEnumerator^ enumerator = osY->GetEnumerator();
			System::Collections::IEnumerator^ enumerator1 = osY1->GetEnumerator();

			y.Clear();
			while(enumerator->MoveNext())
				y.Add(System::Double::Parse(enumerator->Current->ToString()));

			y1.Clear();
			while(enumerator1->MoveNext())
				y1.Add(System::Double::Parse(enumerator1->Current->ToString()));

			Graphics^ g = obraz->CreateGraphics();
			g->Clear(System::Drawing::Color::White);

			Pen^ pioro = gcnew Pen(System::Drawing::Color::Black);
			SolidBrush^ pedzelString = gcnew SolidBrush(System::Drawing::Color::Black);
			System::Drawing::Font^ czcionka = gcnew System::Drawing::Font(System::Drawing::FontFamily::GenericSansSerif, 6, FontStyle::Regular);

			pioro->EndCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;
			// rysowanie osi X
			g->DrawLine(pioro, 30, obraz->Height-20, obraz->Width-30, obraz->Height-20);
			pioro->EndCap = System::Drawing::Drawing2D::LineCap::NoAnchor;
			for(int i=0; i<=10; i++)
			{
				float jednostka = (obraz->Width-70)/10;
				g->DrawLine(pioro, 30+i*jednostka, obraz->Height-20.0F, 30+i*jednostka, obraz->Height-16.0F);
				g->DrawString( (xMin + i*(xMax-xMin)/10).ToString(), czcionka, pedzelString, 25+i*jednostka, obraz->Height-15);
				if(siatka && i!=0)
				{
					pioro->Color = System::Drawing::Color::LightGray;
					g->DrawLine(pioro, 30+i*jednostka, obraz->Height-20.0F, 30+i*jednostka, 10.0F);
					pioro->Color = System::Drawing::Color::Black;
				}
			}

			pioro->EndCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;
			// rysowanie osi Y
			g->DrawLine(pioro, 30, obraz->Height-20, 30, 10);
			pioro->EndCap = System::Drawing::Drawing2D::LineCap::NoAnchor;
			for(int i=0; i<=10; i++)
			{
				float jednostka = (obraz->Height-40)/10;
				g->DrawLine(pioro, 26.0F, obraz->Height-20-i*jednostka, 30.0F, obraz->Height-20-i*jednostka);
				g->DrawString( (yMin + i*(yMax-yMin)/10).ToString(), czcionka, pedzelString, 5, obraz->Height-25-i*jednostka);
				if(siatka && i!=0)
				{
					pioro->Color = System::Drawing::Color::LightGray;
					g->DrawLine(pioro, 31.0F, obraz->Height-20-i*jednostka, obraz->Width-30.0F, obraz->Height-20-i*jednostka);
					pioro->Color = System::Drawing::Color::Black;
				}
			}

			// rysowanie wykresu
			for(int i=0; i<y.Count-1; i++)
			{
				float jednostkaY = (obraz->Height-40)/(yMax-yMin);
				float zeroY = obraz->Height-20 + yMin*jednostkaY;
				float war1 = zeroY - jednostkaY*(double)y[i];
				float war2 = zeroY - jednostkaY*(double)y[i+1];

				float jednostkaX = (obraz->Width-70)/(xMax-xMin);
				float zeroX = 30 - xMin*jednostkaX;

				float zakres = xMax - xMin;
				pioro->Color = System::Drawing::Color::Green;
				g->DrawLine(pioro, zeroX+(obraz->Width-70)*i*kwant/zakres, war1, zeroX+(obraz->Width-70)*(i+1)*kwant/zakres, war2);
			}

			for(int i=0; i<y1.Count-1; i++)
			{
				float jednostkaY = (obraz->Height-40)/(yMax-yMin);
				float zeroY = obraz->Height-20 + yMin*jednostkaY;
				float war1 = zeroY - jednostkaY*(double)y1[i];
				float war2 = zeroY - jednostkaY*(double)y1[i+1];

				float jednostkaX = (obraz->Width-70)/(xMax-xMin);
				float zeroX = 30 - xMin*jednostkaX;

				float zakres = xMax - xMin;
				pioro->Color = System::Drawing::Color::Red;
				g->DrawLine(pioro, zeroX+(obraz->Width-70)*i*kwant/zakres, war1, zeroX+(obraz->Width-70)*(i+1)*kwant/zakres, war2);
			}
		}

	public: void reset()
	{
		y.Clear();
		y1.Clear();
		odswierz();
	}

	//===================================== przedzia³ X
	private:
		double xMax;
	public:
		double getXMax()
		{
			return xMax;
		}
		void setXMax(double value)
		{
			xMax = value;
		}

	private:
		double xMin;
	public:
		double getXMin()
		{
			return xMin;
		}
		void setXMin(double value)
		{
			xMin = value;
		}
	
	//===================================== przedzia³ Y
	private:
		double yMax;
	public:
		double getYMax()
		{
			return yMax;
		}
		void setYMax(double value)
		{
			yMax = value;
		}

	private:
		double yMin;
	public:
		double getYMin()
		{
			return yMin;
		}
		void setYMin(double value)
		{
			yMin = value;
		}

	//===================================== siatka
	private:
		bool siatka;
	public:
		bool getSiatka()
		{
			return siatka;
		}
		void setSiatka(bool value)
		{
			siatka = value;
		}

	//===================================== kwant
	private:
		double kwant;
	public:
		double getKwant()
		{
			return kwant;
		}
		void setKwant(double value)
		{
			kwant = value;
		}

	//===================================== poczatek
	private:
		double poczatek;
	public:
		double getPoczatek()
		{
			return poczatek;
		}
		void setPoczatek(double value)
		{
			poczatek = value;
		}

	private: System::Windows::Forms::PictureBox^  obraz;

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
			this->obraz = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->xMi = (gcnew System::Windows::Forms::NumericUpDown());
			this->yMa = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->yMi = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->xMa = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->obraz))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->xMi))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->yMa))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->yMi))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->xMa))->BeginInit();
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// obraz
			// 
			this->obraz->BackColor = System::Drawing::Color::White;
			this->obraz->Dock = System::Windows::Forms::DockStyle::Top;
			this->obraz->Location = System::Drawing::Point(0, 0);
			this->obraz->Name = L"obraz";
			this->obraz->Size = System::Drawing::Size(386, 120);
			this->obraz->TabIndex = 0;
			this->obraz->TabStop = false;
			this->obraz->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Wykres::obraz_Paint);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 5);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(29, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"xMin";
			// 
			// xMi
			// 
			this->xMi->Location = System::Drawing::Point(38, 2);
			this->xMi->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->xMi->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, -2147483648});
			this->xMi->Name = L"xMi";
			this->xMi->Size = System::Drawing::Size(49, 20);
			this->xMi->TabIndex = 3;
			this->xMi->ValueChanged += gcnew System::EventHandler(this, &Wykres::xMi_ValueChanged);
			// 
			// yMa
			// 
			this->yMa->Location = System::Drawing::Point(314, 3);
			this->yMa->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->yMa->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, -2147483648});
			this->yMa->Name = L"yMa";
			this->yMa->Size = System::Drawing::Size(49, 20);
			this->yMa->TabIndex = 5;
			this->yMa->ValueChanged += gcnew System::EventHandler(this, &Wykres::yMa_ValueChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(276, 5);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(32, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"yMax";
			// 
			// yMi
			// 
			this->yMi->Location = System::Drawing::Point(221, 3);
			this->yMi->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->yMi->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, -2147483648});
			this->yMi->Name = L"yMi";
			this->yMi->Size = System::Drawing::Size(49, 20);
			this->yMi->TabIndex = 7;
			this->yMi->ValueChanged += gcnew System::EventHandler(this, &Wykres::yMi_ValueChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(186, 5);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(29, 13);
			this->label3->TabIndex = 6;
			this->label3->Text = L"yMin";
			// 
			// xMa
			// 
			this->xMa->Location = System::Drawing::Point(131, 2);
			this->xMa->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->xMa->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, -2147483648});
			this->xMa->Name = L"xMa";
			this->xMa->Size = System::Drawing::Size(49, 20);
			this->xMa->TabIndex = 9;
			this->xMa->ValueChanged += gcnew System::EventHandler(this, &Wykres::xMa_ValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(93, 5);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(32, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"xMax";
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->xMa);
			this->panel1->Controls->Add(this->xMi);
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->yMi);
			this->panel1->Controls->Add(this->yMa);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->panel1->Location = System::Drawing::Point(0, 195);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(386, 25);
			this->panel1->TabIndex = 10;
			// 
			// Wykres
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->obraz);
			this->Name = L"Wykres";
			this->Size = System::Drawing::Size(386, 220);
			this->Load += gcnew System::EventHandler(this, &Wykres::Wykres_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->obraz))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->xMi))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->yMa))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->yMi))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->xMa))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void obraz_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
	{
		Graphics^ g = e->Graphics;
		g->Clear(System::Drawing::Color::White);

		Pen^ pioro = gcnew Pen(System::Drawing::Color::Black);
		SolidBrush^ pedzelString = gcnew SolidBrush(System::Drawing::Color::Black);
		System::Drawing::Font^ czcionka = gcnew System::Drawing::Font(System::Drawing::FontFamily::GenericSansSerif, 6, FontStyle::Regular);

		pioro->EndCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;
		// rysowanie osi X
		g->DrawLine(pioro, 30, obraz->Height-20, obraz->Width-30, obraz->Height-20);
		pioro->EndCap = System::Drawing::Drawing2D::LineCap::NoAnchor;
		for(int i=0; i<=10; i++)
		{
			float jednostka = (obraz->Width-70)/10;
			g->DrawLine(pioro, 30+i*jednostka, obraz->Height-20.0F, 30+i*jednostka, obraz->Height-16.0F);
			g->DrawString( (xMin + i*(xMax-xMin)/10).ToString(), czcionka, pedzelString, 25+i*jednostka, obraz->Height-15);
			if(siatka && i!=0)
			{
				pioro->Color = System::Drawing::Color::LightGray;
				g->DrawLine(pioro, 30+i*jednostka, obraz->Height-20.0F, 30+i*jednostka, 10.0F);
				pioro->Color = System::Drawing::Color::Black;
			}
		}

		pioro->EndCap = System::Drawing::Drawing2D::LineCap::ArrowAnchor;
		// rysowanie osi Y
		g->DrawLine(pioro, 30, obraz->Height-20, 30, 10);
		pioro->EndCap = System::Drawing::Drawing2D::LineCap::NoAnchor;
		for(int i=0; i<=10; i++)
		{
			float jednostka = (obraz->Height-40)/10;
			g->DrawLine(pioro, 26.0F, obraz->Height-20-i*jednostka, 30.0F, obraz->Height-20-i*jednostka);
			g->DrawString( (yMin + i*(yMax-yMin)/10).ToString(), czcionka, pedzelString, 5, obraz->Height-25-i*jednostka);
			if(siatka && i!=0)
			{
				pioro->Color = System::Drawing::Color::LightGray;
				g->DrawLine(pioro, 31.0F, obraz->Height-20-i*jednostka, obraz->Width-30.0F, obraz->Height-20-i*jednostka);
				pioro->Color = System::Drawing::Color::Black;
			}
		}

		// rysowanie wykresu
		for(int i=0; i<y.Count-1; i++)
		{
			float jednostkaY = (obraz->Height-40)/(yMax-yMin);
			float zeroY = obraz->Height-20 + yMin*jednostkaY;
			float war1 = zeroY - jednostkaY*(double)y[i];
			float war2 = zeroY - jednostkaY*(double)y[i+1];

			float jednostkaX = (obraz->Width-70)/(xMax-xMin);
			float zeroX = 30 - xMin*jednostkaX;

			float zakres = xMax - xMin;
			pioro->Color = System::Drawing::Color::Green;
			g->DrawLine(pioro, zeroX+(obraz->Width-70)*i*kwant/zakres, war1, zeroX+(obraz->Width-70)*(i+1)*kwant/zakres, war2);
		}

		for(int i=0; i<y1.Count-1; i++)
		{
			float jednostkaY = (obraz->Height-40)/(yMax-yMin);
			float zeroY = obraz->Height-20 + yMin*jednostkaY;
			float war1 = zeroY - jednostkaY*(double)y1[i];
			float war2 = zeroY - jednostkaY*(double)y1[i+1];

			float jednostkaX = (obraz->Width-70)/(xMax-xMin);
			float zeroX = 30 - xMin*jednostkaX;

			float zakres = xMax - xMin;
			pioro->Color = System::Drawing::Color::Red;
			g->DrawLine(pioro, zeroX+(obraz->Width-70)*i*kwant/zakres, war1, zeroX+(obraz->Width-70)*(i+1)*kwant/zakres, war2);
		}
	}

	private: System::Void xMi_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(xMi->Value >= xMa->Value)
			xMi->Value = xMa->Value-1;

		xMin = (int)xMi->Value;
		odswierz();
	}

	private: System::Void xMa_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(xMa->Value <= xMi->Value)
			xMa->Value = xMi->Value+1;
		
		xMax = (int)xMa->Value;
		odswierz();
	}

	private: System::Void yMi_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(yMi->Value >= yMa->Value)
			yMi->Value = yMa->Value-1;

		yMin = (int)yMi->Value;
		odswierz();
	}

	private: System::Void yMa_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(yMa->Value <= yMi->Value)
			yMa->Value = yMi->Value+1;

		yMax = (int)yMa->Value;
		odswierz();
	}

	private: System::Void Wykres_Load(System::Object^  sender, System::EventArgs^  e)
	{
		obraz->Height = this->Height - panel1->Height;
		//MessageBox::Show(obraz->Height.ToString(),"d",MessageBoxButtons::OK);
		xMi->Value = (int)xMin;
		xMa->Value = (int)xMax;
		yMi->Value = (int)yMin;
		yMa->Value = (int)yMax;
	}
};
}
