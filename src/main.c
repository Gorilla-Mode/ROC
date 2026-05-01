#define UNITY_BUILD

#include "global_typedefs.c"
#include "celestial_body.c"
#include "error.h"
#include "orbit.c"
#include "error.c"
#include "orbital_mechanics.c"
#include "planets.c"
#include <curses.h>

typedef struct {
    int32_t selected_body;
    f64_t altitude;
} UIState;

OrbitError orbitErr = 0;
ResonantError resErr = 0;
int32_t precision = 1000;

void DrawBodyList(WINDOW *win, int32_t selected)
{
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Bodies ");

    for (int32_t i = 0; i < BODY_COUNT; ++i)
    {
        if (i == selected) wattron(win, A_REVERSE);

        mvwprintw(win, i + 1, 2, "%s", Kerbol[i].Name);

        if (i == selected) wattroff(win, A_REVERSE);
    }

    wrefresh(win);
}

void DrawBodyInfo(WINDOW *win, int32_t selected)
{
    CelestBody *b = &Kerbol[selected];

    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Selected Body Info ");

    mvwprintw(win, 1, 2, "Name:                %s",   b->Name);
    mvwprintw(win, 2, 2, "Grav Surf:           %.2f m/s^2", b->GravSurf);
    mvwprintw(win, 3, 2, "Grav Param:          %.2f m^3*s^-2", b->GravParam);
    mvwprintw(win, 4, 2, "Sphere of influence: %.2f m", b->SOI);
    mvwprintw(win, 5, 2, "Atmosphere height:   %d m", b->AtmHeightM);
    mvwprintw(win, 6, 2, "Equatorial radius:   %llu m", b->EqRadiusM);

    wrefresh(win);
}

void DrawResults(WINDOW *win, const Orbit *o1, const Orbit *res)
{
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Result ");

    mvwprintw(win, 1, 2, "Resonant apoapsis:  %.2lf m", res->ApoapsisHeight(res));
    mvwprintw(win, 2, 2, "Resonant periapsis: %.2lf m", res->PeriapsisHeight(res));
    mvwprintw(win, 3, 2, "Eccentricity:       %.2lf", res->Eccentricity);

    mvwprintw(win, 5, 2, "Target Period:      %lf s", o1->OPeriod(o1));
    mvwprintw(win, 6, 2, "Resonant Period:    %lf s", res->OPeriod(res));
    mvwprintw(win, 7, 2, "DeltaV:             %.2f m/s",
              DeltaVCircToEllip(o1, res, nullptr));

    mvwprintw(win, 9, 2, "Resonant state:    %s", ErrToStr(resErr));
    mvwprintw(win, 10, 2, "Orbit state:       %s", ErrToStr(orbitErr));
    mvwprintw(win, 11, 2, "LOS state:         %s", ErrToStr(resErr));

    wrefresh(win);
}

void DrawControls(WINDOW *win, f64_t altitude, const Orbit *o1)
{

    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Input ");

    mvwprintw(win, 1, 2, "Altitude:   %.00f m", altitude);
    mvwprintw(win, 2, 2, "Period:     %lf s", o1->OPeriod(o1));
    mvwprintw(win, 3, 2, "Satellites: %d", 40);
    mvwprintw(win, 4, 2, "Mode:       Retrograde");
    mvwprintw(win, 5, 2, "Precision:  %d m", precision);

    wrefresh(win);
}

void DrawFooter(WINDOW *win)
{
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "q = quit | up/down = change body | left/right = change altitude | m = change mode | p = change precision");

    wrefresh(win);
}

void changePrecision()
{
    switch (precision)
    {
        case 1000: precision = 100; break;
        case 100: precision = 10; break;
        case 10: precision = 1; break;
        case 1: precision = 1000; break;
        default: break;
    }
}

int32_t main(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int32_t rows, cols;
    getmaxyx(stdscr, rows, (cols));

    int32_t footer_h    = 3;
    int32_t main_h      = rows - footer_h;

    int32_t left_w      = cols / 2;
    int32_t right_w     = cols - left_w;

    int32_t half_h      = main_h / 2;

    WINDOW *left_top    = newwin(half_h +5, left_w, 0, 0);
    WINDOW *left_bottom = newwin(main_h - half_h -5, left_w, half_h + 5, 0);

    WINDOW *right_top   = newwin(half_h, right_w, 0, left_w);
    WINDOW *right_bot   = newwin(main_h - half_h, right_w, half_h, left_w);

    WINDOW *footer      = newwin(footer_h, cols, main_h, 0);

    UIState state = {0};
    state.selected_body = 0;
    state.altitude = 150000;

    bool running = true;

    refresh();
    doupdate();

    while (running)
    {
        resErr = 0;
        orbitErr = 0;

        Orbit orbit1 = CreateOrbitCircularAlt(
            &Kerbol[state.selected_body],
            state.altitude,
            &orbitErr
        );

        Orbit resOrbit = CalcResonantOrbitRetr(
            &orbit1,
            40,
            &resErr
        );

        DrawBodyList(left_top, state.selected_body);
        DrawBodyInfo(left_bottom, state.selected_body);
        DrawResults(right_top, &orbit1, &resOrbit);
        DrawControls(right_bot, state.altitude, &orbit1);
        DrawFooter(footer);

        int32_t ch = getch();

        switch (ch)
        {
            case 'q':
                running = false;
                break;

            case 'p':
                changePrecision();
            break;

            case KEY_UP:
                state.selected_body =
                    (state.selected_body - 1 + BODY_COUNT) % BODY_COUNT;
                break;

            case KEY_DOWN:
                state.selected_body =
                    (state.selected_body + 1) % BODY_COUNT;
                break;

            case KEY_RIGHT:
                state.altitude += precision;
                break;

            case KEY_LEFT:
                state.altitude -= precision;
                if (state.altitude < precision) state.altitude = precision;
                break;
            default: break;
        }
    }

    endwin();
    return 0;
}