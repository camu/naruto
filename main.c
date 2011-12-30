#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void loadpf( char * );
int gameloop( );

#define __default_pf "def.pf"

#ifdef cheat_code
	char cheat = 0;
#endif

char *pf;
int w, h, score = 1;

int main( int argc, char *argv[] ) {
	if( argc == 2 ) loadpf( argv[1] );
	else loadpf( __default_pf );

	if( system( "mplayer -fs loader.flv &> /dev/null" ) > 0 ) {
		printf( "lolnab\nu cunt pley da vidya so u cunt pley THE GAME!\n" );
		return 1;
	}

	initscr( );
	raw( );
	timeout( 40 );
	noecho( );
	nonl( );
	keypad( stdscr, TRUE );


	int end = gameloop( );

	clear( );
	if( end == 0 ) mvaddstr( 10, 10, "n00b, u lost THE GAME" );
	if( end == 1 ) mvprintw( 10, 10, "gz, u wind! pisteesi: %i", score );
	if( end == 2 ) mvaddstr( 10, 10, "ONNEE! vedit 9001 pistettä eli sait\n          huippusalaisen supervoiton\n          t: naruto" );

	timeout( 5000 );
	getch( );
	endwin( );

	free( pf );

	if( end < 2 ) { int i; for( i = 0; i < 50; i++ ) printf( "THE GAME\n" ); }

	return 0;
}

void loadpf( char *_fname ) {
	FILE *fp = fopen( _fname, "r" );

	{ char c = fgetc( fp ); for( h = 0; c != EOF; c = fgetc( fp ) ) if( c == '\n' ) h++; }
	rewind( fp );
	{ char c = fgetc( fp ); for( w = 0; c != '\n'; c = fgetc( fp ), w++ ); }
	rewind( fp );

	pf = malloc( h*w+1 );
	{
		int j, i;
		char c = fgetc( fp );
		for( j = 0; j < h; j++ ) {
			for( i = 0; i < w; i++, c = fgetc( fp ) ) {
				if( c != '\n' ) {
					pf[j*w+i] = c;
					continue;
				} else {
					pf[j*w+i] = ' ';
					ungetc( c, fp );
				}
			}
			c = fgetc( fp );
		}
	}

	fclose( fp );
}

int gameloop( ) {
	int x, y;
	x = 5, y = 0;
	char canjump = 1, q = 0;
	int c;
	for( ;!q; ) {
		c = getch( );
		switch( c ) {
			case KEY_LEFT:
				if( pf[y*w+x-1] != '#' && x > 0 ) x--;
				break;
			case KEY_RIGHT:
				if( pf[y*w+x+1] != '#' && x < w ) x++;
				break;
			case KEY_DOWN:
				if( canjump ) { y += 10; canjump = 0; }
				break;
			case 'q':
				q = 1;
				break;
#ifdef cheat_code
			case '*':
				if( score == 8001 ) cheat = 1;
#endif
		}

		{ int j, i; for( j = 0; j < h; j++ ) for( i = 0; i < w; i++ ) mvaddch( j, i, pf[j*w+i] ); }

		move( y > 0 ? y-- : 0, x );
		if( y == 0 ) canjump = 1;

		switch( pf[y*w+x] ) {
			case '#':
				y++;
				canjump = 1;
				break;
			case 'V':
				return 1;
			case '*':
#ifndef cheat_code
				return 0;
#else
				if( !cheat ) return 0;
#endif
			case '!':
				pf[y*w+x] = ' ';
				score += 1000;
				if( score == 9001 ) {
					mvaddstr( 15, 10, "ur scoar iz ova 9000!!!!!!!!!!!!!!!!" );
					timeout( 2000 );
					getch( ); mvaddstr( 15, 46, "11111" ); getch( );
					return 2;
				}
		}
	}

	return 0;
}
