#include"Livello.h"

Livello::Livello(const Giocatore& g, vector<Cassa*> c, vector<Muro*> m)
{
	this->inizializzaAllegro();
	muri=m;
	//Inizializzo il livello
	mappa=new int*[12];
	for(int i=0; i<12; i++)
		mappa[i]=new int [10];
	
	for(int i=0; i<12; i++)
		for(int j=0; j<10; j++)
			mappa[i][j] = 0;

	// 0=niente	 1=muri	 2=casse  3=giocatore
	int x=g.getX();
	int y=g.getY();
	mappa[x][y]=3;
	
	for(unsigned i=0; i<c.size(); i++)
	{	
		x=c[i]->getX();
		y=c[i]->getY();		
		mappa[x][y]=2;
	}
	
	for(unsigned i=0; i<m.size(); i++)
	{
		x=m[i]->getX();
		y=m[i]->getY();
		mappa[x][y]=1;
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			cout << mappa[j][i] << " ";
		cout << endl;
	}

	mosse.push(new Mossa(g, c, mappa));		// mossa 0		come inizia il livello

	Wall = al_load_bitmap("wall.png");
	Undo = al_load_bitmap("Indietro.png");
	Rigioca = al_load_bitmap("Ripeti.png");
	Box= al_load_bitmap("box.png");
	BoxColor = al_load_bitmap("boxColor.png");
	Player = al_load_bitmap("George.png");
}
	
bool clear(stack<Mossa*> mosse)
{
	if(mosse.size()<2)
		return false;
	
	while(mosse.size()>1)
	{
		mosse.pop();
	}

	return true;

}

const int undo_x = 512, undo_y = 0, rigioca_x = 576, rigioca_y = 0;
void Livello::drawMap() const
{

	al_draw_bitmap(Undo, undo_x, undo_y, 0);
	al_draw_bitmap(Rigioca, rigioca_x, rigioca_y, 0);

	for (unsigned i = 0; i < muri.size(); i++) {
		al_draw_bitmap(Wall, muri[i]->getX() * 64, muri[i]->getY() * 64, NULL);
	}

	vector<Cassa*> casse=mosse.top()->getCasse();	

	for(unsigned i=0; i<casse.size(); i++)
		al_draw_filled_circle(casse[i]->getEndX()*64+32,casse[i]->getEndY()*64+32, 16, al_map_rgb(255,0,0));						

	for (unsigned i = 0; i < casse.size(); i++) {
		if (casse[i]->getPoint())
			al_draw_bitmap(BoxColor, casse[i]->getX() * 64, casse[i]->getY() * 64, NULL);
		else
			al_draw_bitmap(Box, casse[i]->getX() * 64, casse[i]->getY() * 64, NULL);
	}

	Giocatore player = mosse.top()->getPlayer();
	al_draw_bitmap_region(Player, (player.getDir() * 64), 0, 64, 64, player.getX() * 64, player.getY() * 64, NULL);

	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));
}

void Livello::gioca()
{
	//Posizioni iniziali del livello
	Giocatore player=mosse.top()->getPlayer();
	int x=player.getX();
	int y=player.getY();

	vector<Cassa*> casse=mosse.top()->getCasse();	
	enum DIR {DOWN = 0, LEFT,UP, RIGHT};  
	ALLEGRO_KEYBOARD_STATE keystate;
	
	bool draw=false;

	drawMap();
	
	//inizio timer e game loop
	al_start_timer(timer);

	while (true) 
	{	
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		al_get_keyboard_state(&keystate);

		if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;
      
		if (events.type == ALLEGRO_EVENT_TIMER)
		{
			int player_x=player.getX();
			int player_y=player.getY();
	
			// 0=Aria	 1=Muri	 2=Casse  3=Giocatore
			//Movimenti di Casse e Giocatore
			if (al_key_down(&keystate, ALLEGRO_KEY_DOWN))
			{	
				draw=true;
				player.setDir(DOWN);
				if (mappa[player_x][player_y + 1] == 0)
				{
					player.MovePlayer();
					mappa[player_x][player_y+1] = 3;
					mappa[player_x][player_y] = 0;
				}
				else if (mappa[player_x][player_y + 1] == 2 && mappa[player_x][player_y + 2] == 0) 
				{
					for(unsigned i = 0; i<casse.size(); i++)
						if (casse[i]->getY() == player_y+1 && casse[i]->getX() == player_x)
						{
							casse[i]->sposta(DOWN);
							break;
						}			
					player.MovePlayer();
				
					mappa[player_x][player_y + 1] = 3;
					mappa[player_x][player_y] = 0;
					mappa[player_x][player_y + 2] = 2;
					
				}

			}
			else if (al_key_down(&keystate, ALLEGRO_KEY_UP))
			{
				draw=true;
				player.setDir(UP);
				if (mappa[player_x][player_y - 1] == 0)
				{
					player.MovePlayer();
					mappa[player_x][player_y - 1] = 3;
					mappa[player_x][player_y] = 0;
			
				}
				else if (mappa[player_x][player_y - 1] == 2 && mappa[player_x][player_y - 2] == 0)
				{
					for (unsigned i = 0; i<casse.size(); i++)
						if (casse[i]->getY() == player_y - 1 && casse[i]->getX() == player_x)
						{
							casse[i]->sposta(UP);
							break;
						}
					player.MovePlayer();
					mappa[player_x][player_y - 1] = 3;
					mappa[player_x][player_y] = 0;
					mappa[player_x][player_y - 2] = 2;
				
				}

			}
			else if (al_key_down(&keystate, ALLEGRO_KEY_RIGHT))
			{
				draw=true;
				player.setDir(RIGHT);
				if (mappa[player_x + 1][player_y] == 0)
				{
					player.MovePlayer();
					mappa[player_x + 1][player_y] = 3;
					mappa[player_x][player_y] = 0;
				
				}
				else if (mappa[player_x + 1][player_y] == 2 && mappa[player_x + 2][player_y] == 0)
				{
					for (unsigned i = 0; i<casse.size(); i++)
						if (casse[i]->getY() == player_y && casse[i]->getX() == player_x+1)
						{
							casse[i]->sposta(RIGHT);
							break;
						}
					player.MovePlayer();
					mappa[player_x + 1][player_y] = 3;
					mappa[player_x][player_y] = 0;
					mappa[player_x + 2][player_y] = 2;
		
				}
			}
			else if (al_key_down(&keystate, ALLEGRO_KEY_LEFT))
			{
				draw=true;
				player.setDir(LEFT);
				if (mappa[player_x - 1][player_y] == 0)
				{
					player.MovePlayer();
					mappa[player_x - 1][player_y] = 3;
					mappa[player_x][player_y] = 0;
				}
				else if (mappa[player_x - 1][player_y] == 2 && mappa[player_x - 2][player_y] == 0 )
				{
					for (unsigned i = 0; i<casse.size(); i++)
						if (casse[i]->getY() == player_y && casse[i]->getX() == player_x-1)
						{
							casse[i]->sposta(LEFT);
							break;
						}
					player.MovePlayer();
					mappa[player_x - 1][player_y] = 3;
					mappa[player_x][player_y] = 0;
					mappa[player_x - 2][player_y] = 2;
					
				}
			}

			//redraw della mappa
			if(draw && al_is_event_queue_empty(event_queue))
			{
				draw=false;

				drawMap();
				//Inserimento ultima mossa fatta
				mosse.push(new Mossa(player, casse, mappa));
			}
		}

		//Undo e Rigioca livello
		int a=0, b=0;
		if(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			a=events.mouse.x;
			b=events.mouse.y;
			
			if(a>=rigioca_x && a<=rigioca_x+64 && b>=rigioca_y && b<=rigioca_y+64)
			{
				clear(mosse);
				casse=mosse.top()->getCasse();			
				player=mosse.top()->getPlayer();
				x=player.getX();
				y=player.getY();	
			}
		
			if(a>=undo_x && a<=undo_x+64 && b>=undo_y && b<=undo_y+64 && mosse.size()>1)
			{
				mosse.pop();
				casse.clear();				
				casse=mosse.top()->getCasse();
				player=mosse.top()->getPlayer();
				x=player.getX();
				y=player.getY();
				mappa=mosse.top()->getMappa();
					
				drawMap();
			}
		}

		//Controllo fine livello
		if(Done(casse))
		{
			clear(mosse);
			mosse.pop();
		//	delete muri;
			al_destroy_timer(timer);
			al_destroy_event_queue(event_queue);
			al_destroy_display(display);
			break;
		}
	}

}

bool Livello::Done(vector<Cassa*> casse) const
{
	for (unsigned i = 0; i < casse.size(); i++) {
		int x = casse[i]->getEndX();
		int y = casse[i]->getEndY();
		if (mappa[x][y] != 2)
			return false;
	}
	return true;
}

void Livello::inizializzaAllegro()
{
	if(!al_init())
	{
		cerr<<"no allegro"<<endl;
	}

	if(!al_init_image_addon())
	{ 
	  cerr<<"failed initialisation image"<<endl;
	}
	
	if(!al_init_primitives_addon())
	{ 
	  cerr<<"failed initialisation primitives"<<endl;
	}

	if(!al_init_native_dialog_addon())
	{ 
	  cerr<<"failed initialisation dialog"<<endl;
	}

	if(!al_install_keyboard())
	{
		cerr<<"no keyboard"<<endl;
	}
	
	if(!al_install_mouse())
	{
		cerr<<"no mouse"<<endl;
	}		
	
	display=al_create_display(800,700);

	if(!display)
	{
		cerr<<"no display"<<endl;
	}
	al_set_window_title(display,"Sokoban");

	const float FPS=10;
	timer=al_create_timer(1/FPS);
	if(!timer)
	{
		cerr<<"no timer"<<endl;
	}

	event_queue=al_create_event_queue();
	if(!event_queue)
	{
		cerr<<"no event_queue"<<endl;
	}
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());		
}


