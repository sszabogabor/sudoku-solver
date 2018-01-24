#include "sudokuimpl.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent> 
#include <QTextStream>
#include <QVector>
#include <string>

int zalohaG[9][9];
int reseniG[9][9];

SudokuImpl::SudokuImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
{
	ui.setupUi(this);
	
	connect( ui.pushButton , SIGNAL(clicked()), this, SLOT(solve()) );
	connect( ui.actionOpen, SIGNAL(activated()), this, SLOT(openFile()) );
	connect( ui.actionQuit, SIGNAL(activated()), this, SLOT(closeWindow()) );
	connect( ui.actionSave, SIGNAL(activated()), this, SLOT(saveFile()) );
	
	setUpdatesEnabled( TRUE );
	
}

void SudokuImpl::openFile()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Open Sudoku text file"),
		QDir::currentPath(),
		tr("Document files (*.txt);;All files (*.*)") );
		
	QTextStream szoveg;
	if( !filename.isNull() )
	{

		QFile file(filename);
		if ( !file.open(QIODevice::ReadWrite | QIODevice::Text ))
		{
			QMessageBox msgbx ;
			msgbx.setText( "Could not open file" );
			msgbx.exec();
		} else {
			ui.label1->setText(filename);	
			QTextStream stream( &file );
			int i=0;
			QVector<QString> myVector;
				while ( !stream.atEnd() )
				{
				QString line;
				line=stream.readLine();
					if ( line=="sudoku"){
						i=1;						
					}					
					if (i>0 && i<=10){
						if ( line!="sudoku" ){
							myVector.push_back(line);
							//ui.listWidget->addItem(line);
							++i;
						}
						
					}
					
				}
				/*for (int j=0;j<myVector.size(); ++j)
					{
						ui.listWidget->addItem(myVector.at(j));
				}*/
			writeToList(myVector);
			
			file.close();
		}
	}
}

void SudokuImpl::saveFile(){
	QString filename = QFileDialog::getSaveFileName(this,tr("Save Sudoku text file"),QDir::currentPath(),tr("Document files (*.txt);;All files (*.*)") );
	
	if( !filename.isNull() )
	{

		QFile file(filename);
		if ( !file.open(QIODevice::WriteOnly | QIODevice::Text ))
		{
			QMessageBox msgbx ;
			msgbx.setText( "Could not open file" );
			msgbx.exec();
		} else {
			ui.label1->setText(filename);	
			QTextStream szoveg (&file);
			
			szoveg << "sudoku";
			
			for (int i=0;i<9;++i){
				QString pom;
				for (int j=0;j<9;++j){
					QString pom1 = QString::number(zalohaG[i][j]);
					pom=pom+pom1;
				}
				
				szoveg << "\n";
				szoveg << pom;
			}
			szoveg << "\n";
			szoveg << "\n";
			szoveg << "solution";
			
			for (int i=0;i<9;++i){
				QString pom;
				for (int j=0;j<9;++j){
					QString pom1 = QString::number(reseniG[i][j]);
					pom=pom+pom1;
				}
				
				szoveg << "\n";
				szoveg << pom;
			}
			
			file.close();
		}
	}
}


void SudokuImpl::writeToList(QVector<QString> myVector)
{
	for (int j=0;j<myVector.size(); ++j)
	{
		ui.listWidget->addItem(myVector.at(j));
	}
}

void SudokuImpl::writeToProgressDialog(QVector<QString> myVector)
{
	for (int j=0;j<myVector.size(); ++j)
	{
		ui.progressDialog->addItem(myVector.at(j));
	}
	ui.progressDialog->addItem("-");
	//ui.progressDialog->update();
	//update();
	//repaint();

}

void SudokuImpl::closeEvent(QCloseEvent *event)
 {
     //if (maybeSave()) {
     //    writeSettings();
      //   event->accept();
     //} else {
     //    event->ignore();
     //ui.centralwidget->close();
     
     //}
 }

void SudokuImpl::prBar(){
	double currValue;
	currValue=ui.progressBar->value();
	if (currValue<2000){
		++currValue;
	}
	else{
		ui.progressBar->reset();
		currValue=0;
	}
		ui.progressBar->setValue(currValue);
	
}
	

void SudokuImpl::closeWindow()
{
	switch( QMessageBox::question(
		this,
		tr("Sudoku"),
		tr("Do you really want to exit?"),
		
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
		QMessageBox::Cancel ))
		{
			case QMessageBox::Yes:
				
				break;
			case QMessageBox::No:
				break;
			case QMessageBox::Cancel:
				break;
		}
}


void SudokuImpl::solve()
{
	ui.pushButton->setEnabled(false);

	int zaloha[9][9];
	int reseni[9][9];
	bool r[10];
	bool s[10];
	bool k[10];
	QVector<QString> store; 
	QString pom;
	
	for (int i=0;i<9;++i){
		pom=ui.listWidget->item(i)->text();
		//QScriptValue teszt;
		for (int j=0; j<9;++j){
				zaloha[i][j]=pom.mid(j,1).toInt();
				reseni[i][j]=pom.mid(j,1).toInt();
				zalohaG[i][j]=pom.mid(j,1).toInt();
		}
	}
//solver
	int radek;
	int sloupec;
	int jj=0;
	int cislo;
	bool uz_ne;
	int sloupec__;
	for ( radek=0; radek<9; ++radek){
		for ( sloupec=0; sloupec<9; ++sloupec){
			uz_ne = false;
			if (reseni[radek][sloupec] == 0 ){
			
				cislo=reseni[radek][sloupec];
jdeme:
				for ( jj=0; jj<10 ; ++jj){
					
znovu:					
					if (jj == 9){
//znovu1:						
						if (sloupec == 8 && radek == -1){
							//QMessageBox msg;
							//msg.setText("Unsolvable");
							//msg.exec();
							ui.listWidget->addItem("Unsolvable");
							goto end__;
						}
						--sloupec;
						if (sloupec<0){
							sloupec = 8;
							--radek;
						}
						
						if (zaloha[radek][sloupec] != 0){
							 goto znovu; 
						}
						
						reseni[radek][sloupec] = 0;
						
						jj=0;
						cislo=reseni[radek][sloupec];
						goto jdeme;
					
					}
					
					++cislo;
					
					if (cislo == 10 ){
						cislo=1;
					}
					
					r[cislo] = false;
					s[cislo] = false;
					k[cislo] = false;
					//radek
					int skuska;
					for (int d=0;d<9;++d){
						skuska = reseni[radek][d];
						if ( cislo == reseni[radek][d]) {
							r[cislo] = true;
						}
					}
					//sloupec
					for (int d=0;d<9;++d){
						if ( cislo == reseni[d][sloupec]){
							s[cislo] = true;
						}
					}
					//kocka
					switch (radek){
                            case 0:
                            case 1:
                            case 2:
                                switch (sloupec){
		                            case 0:
		                            case 1:
 			                        case 2:
 			                        	for (int d=0; d<3;++d){
 			                        		for (int dd=0; dd<3;++dd){
                                                if ( cislo == reseni[d][dd]){
                                                	k[cislo] = true;
												} 
                                            }
                                        }
                                        break;
			                         case 3:
			                         case 4:
		        	                 case 5:
 			                        	for (int d=0; d<3;++d){
 			                        		for (int dd=3; dd<6;++dd){
                                                if ( cislo == reseni[d][dd]){
                                                	k[cislo] = true;
												} 
                                            }
                                        }
                                        break;
		                            case 6:
		                            case 7:
		                            case 8:
 			                        	for (int d=0; d<3;++d){
 			                        		for (int dd=6; dd<9;++dd){
                                                if ( cislo == reseni[d][dd]){
                                                	k[cislo] = true;
												} 
                                            }
                                        }
                                        break;
                            	break;
                           		}
                           	break;
                            case 3:
                            case 4:
                            case 5:
                                switch (sloupec){
		                            case 0:
		                            case 1:
 			                        case 2:
 			                        	for (int d=3; d<6;++d){
 			                        		for (int dd=0; dd<3;++dd){
                                                if ( cislo == reseni[d][dd]){
                                                	k[cislo] = true;
												} 
                                            }
                                        }
                                        break;
			                         case 3:
			                         case 4:
		        	                 case 5:
 			                        	for (int d=3; d<6;++d){
 			                        		for (int dd=3; dd<6;++dd){
                                                if ( cislo == reseni[d][dd]){
                                                	k[cislo] = true;
												} 
                                            }
                                        }
                                        break;
		                            case 6:
		                            case 7:
		                            case 8:
 			                        	for (int d=3; d<6;++d){
 			                        		for (int dd=6; dd<9;++dd){
                                                if ( cislo == reseni[d][dd]){
                                                	k[cislo] = true;
												} 
                                            }
                                        }
                                        break;
                            	break;
                           		}
                           	break;
                            case 6:
                            case 7:
                            case 8:
                                switch (sloupec){
		                            case 0:
		                            case 1:
 			                        case 2:
 			                        	for (int d=6; d<9;++d){
 			                        		for (int dd=0; dd<3;++dd){
                                                if ( cislo == reseni[d][dd]){
                                                	k[cislo] = true;
												} 
                                            }
                                        }
                                        break;
			                         case 3:
			                         case 4:
		        	                 case 5:
 			                        	for (int d=6; d<9;++d){
 			                        		for (int dd=3; dd<6;++dd){
                                                if ( cislo == reseni[d][dd]){
                                                	k[cislo] = true;
												} 
                                            }
                                        }
                                        break;
		                            case 6:
		                            case 7:
		                            case 8:
 			                        	for (int d=6; d<9;++d){
 			                        		for (int dd=6; dd<9;++dd){
                                                if ( cislo == reseni[d][dd]){
                                                	k[cislo] = true;
												} 
                                            }
                                        }
                                        break;
                            	break;
                           		}
                           	break;
					}
						prBar();
						
						if (r[cislo] == false && s[cislo] == false && k[cislo] == false) {
	                        //'stejnicislo(radek, sloupec) = cislo
							QString ulozeni_str;
	                        for ( int pom1 = 0; pom1<radek+1; ++pom1){		//<=
	                            if ( pom1 == radek){
	                                sloupec__ = sloupec - 1;
	                            } else {
	                                sloupec__ = 8;
	                            }
	                            
	                            for ( int pom2 = 0; pom2<sloupec__+1; ++pom2){
	                           		QString test_text = QString::number(reseni[pom1][pom2]);
	                           		
		                      	    if (pom2==0 && pom1==0){
		                      	   		ulozeni_str = test_text;
		                     	   	}
		                     	   	else {
		                     	   		ulozeni_str = ulozeni_str + test_text;		
                    	   			}
	                           }
	                        }
	                        
	                       QString cislo_str = QString::number(cislo);
	                       //QString ulozeni_str;
	                       
	                       if (ulozeni_str==""){
		                      	   		ulozeni_str = cislo_str;
		                     	   	}
		                     	   	else {
		                     	   		ulozeni_str = ulozeni_str + cislo_str;		
                    	   			}
                    	   			
	                       int aaa;
	                       aaa=store.size();
	                       for ( int pom1=0; pom1<aaa; ++pom1){         //<=
	                       			QString test;
	                       	 		test=store.at(pom1);
	                       	 	 	if (ulozeni_str == store.at(pom1)){
	                       	 		
	                       	 		int delka = store.at(pom1).size(); //>?
	                       	 		
	                       	 		cislo = store.at(pom1).mid(delka-1,1).toInt();
	                       	 		//cislo = Mid(store(pom1), Len(store(pom1)), 1)
	                       	 		goto znovu;
	                      	 		}
	                     	}
	
	                         reseni[radek][sloupec] = cislo;
	                         ulozeni_str="";
	                         for (int pom1 = 0; pom1<radek+1; ++pom1){
	                              if (pom1 == radek){
	                                   sloupec__ = sloupec;
	                      		  }   
	                      		  else {
	                                  sloupec__ = 8;
		                          }
	                              for (int pom2 = 0; pom2 < sloupec__+1; ++pom2){
	                              	
	                              	QString test_text = QString::number(reseni[pom1][pom2]);
		                      	   if (pom2==0 && pom1==0){
		                      	   	ulozeni_str = test_text;
		                     	   	}
		                     	   	else {
		                     	   		ulozeni_str = ulozeni_str + test_text;		
                    	   			}
                              	  }

                      		 }

	                      	store.push_back(ulozeni_str);
	                      	writeToProgressDialog(store);
	                        uz_ne = true;	 

	                        break;
                        }	// if (r[cislo] == false && s[cislo] == false && k[cislo] == false)

				} //jj end
			} // if (reseni[radek][sloupec] == 0 ){
				
				
				
			if ( uz_ne == false ){
					
					QString ulozeni_str;
                    for ( int pom1=0; pom1<radek+1; ++pom1) {
                       if ( pom1 == radek) {
                       	sloupec__ = sloupec;
                       } else {
                       	
                       	sloupec__ = 8;
                       }
                       for( int pom2 = 0;pom2< sloupec__+1; ++pom2){
						  
                      	   QString test_text = QString::number(reseni[pom1][pom2]);
                      	   //test_text = reseni[pom1][pom2];
                      	   if (pom2==0 && pom1==0){
                      	   	ulozeni_str = test_text;
                     	   	}
                     	   	else {
                     	   		ulozeni_str = ulozeni_str + test_text;		
                    	   	}
                           
                      	}
                    }
                    store.push_back(ulozeni_str);
                    writeToProgressDialog(store);
             }
                
                
		} // sloupec end
	} // rade end

// zapis do listWidget	
	ui.listWidget->addItem("Slolution:");
	
	for (int j=0;j<9; ++j)
	{
		QString solutIon;
		solutIon="";
		for (int k=0;k<9; ++k)
		{
			QString cislo_str = QString::number(reseni[j][k]);
			solutIon=solutIon+cislo_str;
			reseniG[j][k]=reseni[j][k];
		}
		ui.listWidget->addItem(solutIon);
	}
	
	end__:;
	ui.pushButton->setEnabled(true);
}



//


