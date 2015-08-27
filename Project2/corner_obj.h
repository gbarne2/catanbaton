#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CatanBaton {

	/// <summary>
	/// Summary for MyUserControl
	/// </summary>
	public ref class MyUserControl : public System::Windows::Forms::UserControl
	{
	public:
		MyUserControl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyUserControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: Microsoft::VisualBasic::PowerPacks::ShapeContainer^  shapeContainer1;
	protected: 
	private: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape1;
	private: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape2;
	private: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape3;
	private: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape6;
	private: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape5;
	private: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape4;

	protected: 

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
			this->shapeContainer1 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
			this->rectangleShape1 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
			this->rectangleShape2 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
			this->rectangleShape3 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
			this->rectangleShape4 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
			this->rectangleShape5 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
			this->rectangleShape6 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
			this->SuspendLayout();
			// 
			// shapeContainer1
			// 
			this->shapeContainer1->Location = System::Drawing::Point(0, 0);
			this->shapeContainer1->Margin = System::Windows::Forms::Padding(0);
			this->shapeContainer1->Name = L"Tile1";
			this->shapeContainer1->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(6) {this->rectangleShape6, 
				this->rectangleShape5, this->rectangleShape4, this->rectangleShape3, this->rectangleShape2, this->rectangleShape1});
			this->shapeContainer1->Size = System::Drawing::Size(238, 131);
			this->shapeContainer1->TabIndex = 0;
			this->shapeContainer1->TabStop = false;
			// 
			// rectangleShape1
			// 
			this->rectangleShape1->AccessibleName = L"CornerB";
			this->rectangleShape1->BackColor = System::Drawing::Color::Green;
			this->rectangleShape1->Location = System::Drawing::Point(0, 5);
			this->rectangleShape1->Name = L"rectangleShape1";
			this->rectangleShape1->Size = System::Drawing::Size(75, 23);
			this->rectangleShape1->Click += gcnew System::EventHandler(this, &MyUserControl::rectangleShapeB_Click);
			// 
			// rectangleShape2
			// 
			this->rectangleShape2->AccessibleName = L"CornerA";
			this->rectangleShape2->BackColor = System::Drawing::Color::Green;
			this->rectangleShape2->Location = System::Drawing::Point(1, 54);
			this->rectangleShape2->Name = L"rectangleShape2";
			this->rectangleShape2->Size = System::Drawing::Size(75, 23);
			this->rectangleShape2->Click += gcnew System::EventHandler(this, &MyUserControl::rectangleShapeA_Click);
			// 
			// rectangleShape3
			// 
			this->rectangleShape3->AccessibleName = L"CornerF";
			this->rectangleShape3->BackColor = System::Drawing::Color::Green;
			this->rectangleShape3->Location = System::Drawing::Point(1, 100);
			this->rectangleShape3->Name = L"rectangleShape3";
			this->rectangleShape3->Size = System::Drawing::Size(75, 23);
			this->rectangleShape3->Click += gcnew System::EventHandler(this, &MyUserControl::rectangleShapeF_Click);
			// 
			// rectangleShape4
			// 
			this->rectangleShape4->AccessibleName = L"CornerC";
			this->rectangleShape4->BackColor = System::Drawing::Color::Green;
			this->rectangleShape4->Location = System::Drawing::Point(159, 101);
			this->rectangleShape4->Name = L"rectangleShape4";
			this->rectangleShape4->Size = System::Drawing::Size(75, 23);
			this->rectangleShape4->Click += gcnew System::EventHandler(this, &MyUserControl::rectangleShapeE_Click);
			// 
			// rectangleShape5
			// 
			this->rectangleShape5->AccessibleName = L"CornerD";
			this->rectangleShape5->BackColor = System::Drawing::Color::Green;
			this->rectangleShape5->Location = System::Drawing::Point(159, 55);
			this->rectangleShape5->Name = L"rectangleShape5";
			this->rectangleShape5->Size = System::Drawing::Size(75, 23);
			this->rectangleShape5->Click += gcnew System::EventHandler(this, &MyUserControl::rectangleShapeD_Click);
			// 
			// rectangleShape6
			// 
			this->rectangleShape6->AccessibleName = L"CornerE";
			this->rectangleShape6->BackColor = System::Drawing::Color::Green;
			this->rectangleShape6->Location = System::Drawing::Point(158, 6);
			this->rectangleShape6->Name = L"rectangleShape6";
			this->rectangleShape6->Size = System::Drawing::Size(75, 23);
			this->rectangleShape6->Click += gcnew System::EventHandler(this, &MyUserControl::rectangleShapeC_Click);
			// 
			// MyUserControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Lime;
			this->Controls->Add(this->shapeContainer1);
			this->Name = L"MyUserControl";
			this->Size = System::Drawing::Size(238, 131);
			this->Load += gcnew System::EventHandler(this, &MyUserControl::MyUserControl_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void MyUserControl_Load(System::Object^  sender, System::EventArgs^  e) {
			 }

	private: System::Void rectangleShapeB_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void rectangleShapeA_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void rectangleShapeF_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void rectangleShapeC_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void rectangleShapeD_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void rectangleShapeE_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
};
}
