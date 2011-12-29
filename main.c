#include <curses.h>
#include <stdlib.h>
#include <string.h>

void loadpf( char *_fname );
int gameloop( );

#define __default_pf "def.pf"

char *pf;
int w, h, score = 1;

int main( int argc, char *argv[] ) {
	initscr( );
	raw( );
	timeout( 40 );
	noecho( );
	nonl( );
	keypad( stdscr, TRUE );

	if( argc == 2 ) loadpf( argv[1] );
	else loadpf( __default_pf );

	int end = gameloop( );

	clear( );
	if( end == 0 ) mvaddstr( 10, 10, "n00b, u lost the game" );
	if( end == 1 ) mvprintw( 10, 10, "gz, u wind! pisteesi: %i", score );
	if( end == 2 ) mvaddstr( 10, 10, "ONNEE! vedit 9001 pistettä eli sait\n          huippusalaisen supervoiton\n          t: naruto" );

	timeout( 5000 );
	getch( );
	endwin( );

	free( pf );

	return 0;
}

void loadpf( char *_fname ) {
	FILE *fp = fopen( _fname, "r" );

	fseek( fp, 0, SEEK_END );
	int l = ftell( fp );
	rewind( fp );

	char c;
	pf = malloc( l+1 );
	{ int i; for( i = 0; i < l; i++ ) { c = fgetc( fp ); ( c != '\n' ) ? ( pf[i] = c ) : ( i--, h++ ); } }
	pf = realloc( pf, strlen( pf )+1 );

	rewind( fp );
	for( w = 0; w < l; w++ ) if( fgetc( fp ) == '\n' ) break;

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
				return 0;

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
