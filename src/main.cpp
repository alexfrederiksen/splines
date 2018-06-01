#include <exception>
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>

#include <SDL2/SDL.h>
#include "Spline.h"


#ifdef ENABLE_DEBUG_MACRO
#   define DEBUG true
#else
#   define DEBUG false
#endif

static const float SPLINE_SEG_SIZE = 0.05;
static const int POINT_SIZE = 10;

class InitError : public std::exception
{
    std::string msg;
public:
    InitError();
    InitError( const std::string & );
    virtual ~InitError() throw();
    virtual const char * what() const throw();
};

InitError::InitError() :
    exception(),
    msg( SDL_GetError() )
{
}

InitError::InitError( const std::string & m ) :
    exception(),
    msg( m )
{
}

InitError::~InitError() throw()
{
}

const char * InitError::what() const throw()
{
    return msg.c_str();
}

const int dim = 2;

void print_spline(Spline & spline) {
    for (int i = 0; i < spline.get_section_count(); i++) {
        cubic_t & seg = spline.get_sections()[i];
    }
}

class SDL {
    Spline spline;
    std::vector<float> points;

    SDL_Window * m_window;
    SDL_Renderer * m_renderer;
    bool quit = false;
public:
    SDL( Uint32 flags = 0 );
    virtual ~SDL();
    void loop();
    void draw();
    void update();
    void clear();
    void addPoint(int x, int y);
    void computeSpline();
};

SDL::SDL( Uint32 flags ) : spline(2) {
    if ( SDL_Init( flags ) != 0 )
        throw InitError();

    if ( SDL_CreateWindowAndRenderer( 640, 480, SDL_WINDOW_SHOWN,
                                      &m_window, &m_renderer ) != 0 )
        throw InitError();

    addPoint(320, 240);
}

SDL::~SDL()
{
    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    SDL_Quit();
}

void SDL::loop() {
    while (!quit) {
        update();
        draw();
        SDL_Delay(33);
    }

}

void SDL::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                /* add a point to the spline */
                printf("Adding point (%d, %d)... \n", event.button.x, event.button.y);
                addPoint(event.button.x, event.button.y);
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                clear();
            }
        }
    }
}

void SDL::clear() {
    points.clear();
}

void SDL::draw() {
    // Clear the window with a black background
    SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 255 );
    SDL_RenderClear( m_renderer );

    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    /* draw all them segments */
    if (spline.get_section_count() > 0) {
        float point1[2];
        float point2[2];
        for (float t = 0.0; t < spline.get_section_count() - SPLINE_SEG_SIZE; t += SPLINE_SEG_SIZE) {
            if (spline.get_point(t, point1) == nullptr) printf("Theres a probem!! \n");
            if (spline.get_point(t + SPLINE_SEG_SIZE, point2) == nullptr) printf("Theres a problem!! \n");
            SDL_RenderDrawLine(m_renderer, (int) point1[0], (int) point1[1],
                (int) point2[0], (int) point2[1]);
        }
    }
    /* draw all points */
    for (int i = 0; i < points.size(); i += 2) {
        SDL_Rect rect = {
            (int) (points[i] - 0.5 * POINT_SIZE),
            (int) (points[i + 1] - 0.5 * POINT_SIZE),
            POINT_SIZE, POINT_SIZE };
        SDL_RenderDrawRect(m_renderer, &rect);
    }

    // Show the window
    SDL_RenderPresent( m_renderer );
}

void SDL::addPoint(int x, int y) {
    points.push_back((float) x);
    points.push_back((float) y);
    computeSpline();
}

void SDL::computeSpline() {
    spline.compute_spline(&points[0], points.size() / 2);
}

int main( int argc, char * argv[] )
{
    if (DEBUG) std::cout << "Hey I'm debugging now !!! \n\n";

    if (DEBUG) {
        std::cout << "---- [1d Spline Testing] ----" << std::endl;
        int point_len = 4;
        float points[point_len] = { 0.5, 1, 2.6, 2 };
        std::cout << "  Control points: ";
        for (int i = 0; i < point_len; i++) {
            std::cout << points[i] << " ";
        }
        std::cout << "\n\n";

        cubic_t cubics[point_len - 1];
        Spline::compute_simple_spline(points, point_len, 1, 0, cubics);

        for (int i = 0; i < point_len - 1; i++) {
            printf("  cubic_t: %5.2f + %5.2ft + %5.2ft^2 + %5.2ft^3 \n",
                cubics[i].a,
                cubics[i].b,
                cubics[i].c,
                cubics[i].d);
        }
    }


    try
    {
        SDL sdl( SDL_INIT_VIDEO | SDL_INIT_TIMER );
        sdl.loop();

        return 0;
    }
    catch ( const InitError & err )
    {
        std::cerr << "Error while initializing SDL:  "
                  << err.what()
                  << std::endl;
    }

    return 1;
}
