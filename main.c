#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Naruto {
	int x, y;
	char canjump;
} naruto;

int game( char * );
int loadpf( char * );
int gameloop( );

#define __default_pf "def.pf"

#ifdef cheat_code
	char cheat;
#endif

char *pf = NULL;
int w = 0, h = 0, totalscore = 0, score;

naruto *narutos = NULL;
int nnarutos = 0;

int main( int argc, char *argv[] ) {
#ifndef no_loader
	if( system( "mplayer -fs loader.flv &> /dev/null" ) > 0 ) {
		printf( "lolnab\nu cunt pley da vidya so u cunt pley THE GAME!\n" );
		return 1;
	}
#endif

	initscr( );
	raw( );
	noecho( );
	nonl( );
	keypad( stdscr, TRUE );

	if( argc > 1 ) { int i; for( i = 1; i < argc; i++ ) game( argv[i] ); }
	else game( __default_pf );

	clear( );
	timeout( 5000 );
	int lvls = ( argc == 1 ? 1 : argc-1 );
	if( totalscore/lvls > 9000 ) {
		mvaddstr( 10, 10, "Nappa:  VEGETA, WHAT DOES YOUR SCOUTER SAY ABOUT HIS SCORE-PER-LEVEL VALUE?" ); getch( );
		mvaddstr( 11, 10, "Vegeta: IT'S OVER NINE-THOUSAAAAAAAAAAND!!!!!!!!" ); getch( );
		mvaddstr( 12, 10, "Nappa:  WHAT NINE-THOUSAND!? THERE'S NO WAY THAT CAN BE RIGHT, CAN IIIIIT!?!?" ); getch( );
		mvaddstr( 13, 10, "You:    I think it's right." ); getch( );
		mvprintw( 15, 10, "ur tutel scoar s lyk %i. gz.", totalscore ); timeout( 10000 ); getch( );
	} else { mvprintw( 10, 10, "totaalipisteesi: %i", totalscore ); getch( ); }

	endwin( );

	free( pf );
	free( narutos );

	return 0;
}

int game( char *_fname ) {
	if( loadpf( _fname ) ) {
		printf( "file fails\n" );
		return 1;
	}

#ifdef cheat_code
	cheat = 0;
#endif
	score = 1;

	timeout( 40 );
	clear( );

	int end = gameloop( );
	totalscore += score;

	clear( );
	switch( end ) {
		case 0:
			mvaddstr( 10, 10, "n00b, u lost THE GAME" );
			break;
		case 1:
			mvprintw( 10, 10, "gz, u wind! pisteesi: %i", score );
			break;
		case 2:
			mvaddstr( 10, 10, "ONNEE! vedit 9001 pistettä eli sait\n          huippusalaisen supervoiton\n          t: naruto" );
	}

	timeout( 5000 );
	getch( );

	return 0;
}

int loadpf( char *_fname ) {
	FILE *fp = fopen( _fname, "r" );
	if( fp == NULL ) return 1;

	{ char c = fgetc( fp ); for( h = 0; c != EOF; c = fgetc( fp ) ) if( c == '\n' ) h++; }
	rewind( fp );
	{ char c = fgetc( fp ); for( w = 0; c != '\n'; c = fgetc( fp ), w++ ); }
	rewind( fp );

	pf = malloc( (h+10)*w+1 );
	nnarutos = 0;
	{ int j, i;
		char c = fgetc( fp );
		for( j = 0; j < h; j++ ) {
			for( i = 0; i < w; i++, c = fgetc( fp ) ) {
				if( c != '\n' ) {
					if( c == 'X' ) {
						pf[j*w+i] = ' ';
						narutos = realloc( narutos, (nnarutos+1)*sizeof( naruto ) );
						narutos[nnarutos].y = j;
						narutos[nnarutos].x = i;
						narutos[nnarutos].canjump = 1;
						nnarutos++;
					} else pf[j*w+i] = c;
					continue;
				} else {
					pf[j*w+i] = ' ';
					ungetc( c, fp );
				}
			}
			c = fgetc( fp );
		}
		h += 10;
		for( j = h-10; j < h; j++ ) for( i = 0; i < w; i++ ) pf[j*w+i] = ' ';
	}
	if( nnarutos == 0 ) return 1;

	fclose( fp );

	return 0;
}

int gameloop( ) {
	char q = 0, dir = 0;
	int c;
	for( ;!q; ) {
		move( h, w+1 );
		c = getch( );
		switch( c ) {
			case KEY_LEFT:
				dir = -1;
				{ int i; for( i = 0; i < nnarutos; i++ ) if( pf[ narutos[i].y*w+narutos[i].x-1 ] != '#' && narutos[i].x > 0 ) narutos[i].x--; }
				break;
			case KEY_RIGHT:
				dir = 1;
				{ int i; for( i = 0; i < nnarutos; i++ ) if( pf[ narutos[i].y*w+narutos[i].x+1] != '#' && narutos[i].x+1 < w ) narutos[i].x++; }
				break;
			case KEY_DOWN:
				{ int i; for( i = 0; i < nnarutos; i++ ) if( narutos[i].canjump ) { narutos[i].y += 10; narutos[i].canjump = 0; } }
				break;
			case 'q':
				q = 1;
				break;
#ifdef cheat_code
			case '*':
				if( score == 8001 ) cheat = 1;
#endif
		}
		{ int i, j; for( i = 0; i < nnarutos; i++ ) for( j = 0; j < nnarutos; j++ ) if( i != j && narutos[i].x == narutos[j].x ) narutos[i].x -= dir; }

		{ int j, i; for( j = 0; j < h; j++ ) for( i = 0; i < w; i++ ) mvaddch( j, i, pf[j*w+i] ); }

		{ int i;
			for( i = 0; i < nnarutos; i++ ) {
				mvaddch( narutos[i].y > 0 ? narutos[i].y-- : 0, narutos[i].x, 'X' );
				if( narutos[i].y == 0 ) narutos[i].canjump = 1;

				switch( pf[ narutos[i].y*w+narutos[i].x ] ) {
					case '#':
						narutos[i].y++;
						narutos[i].canjump = 1;
						break;
					case 'V':
						return 1;
					case '*':
#ifndef cheat_code
						return 0;
#else
						if( !cheat ) return 0;
						break;
#endif
					case '!':
						pf[ narutos[i].y*w+narutos[i].x ] = ' ';
						score += 1000;
						if( score == 9001 ) {
							mvaddch( narutos[i].y, narutos[i].x, ' ' );
							mvaddstr( 15, 10, "ur scoar iz ova 9000!!!!!!!!!!!!!!!!" );
							timeout( 2000 );
							getch( ); mvaddstr( 15, 46, "11111" ); getch( );
							return 2;
						}
				}
			}
		}
	}

	return 0;
}
