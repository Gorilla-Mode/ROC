#define UNITY_BUILD

#include "global_typedefs.c"
#include "celestial_body.c"
#include "error.h"
#include "orbit.c"
#include "error.c"
#include "orbital_mechanics.c"
#include "planets.c"
#include <curses.h>

typedef enum
{
    MODE_PROGRADE,
    MODE_RETROGRADE
} ResonantMode;

typedef enum
{
    COL_GREEN = 1,
    COL_RED
}TextColor;

typedef struct {
    int32_t selected_body;
    f64_t altitude;
    ResonantMode resMode;
    OrbitError orbitErr;
    ResonantError resErr;
    LosError losErr;
    int32_t precision;
    uint32_t satelliteCount;
} UIState;

typedef Orbit (*ResonantFunc)(const Orbit *orbit, uint32_t satelliteCount, ResonantError* err);

ResonantFunc GetResonantFunc(ResonantMode mode)
{
    switch (mode)
    {
        case MODE_PROGRADE: return CalcResonantOrbitProg;
        case MODE_RETROGRADE: return CalcResonantOrbitRetr;
        default: return nullptr;
    }
}

char* ResonantModeToString(ResonantMode mode)
{
    switch (mode)
    {
        case MODE_PROGRADE:   return "Prograde";
        case MODE_RETROGRADE: return "Retrograde";
        default:              return "Unknown";
    }
}

void DrawBodyList(WINDOW *win, int32_t selected)
{
    werase(win);
    box(win, 0, 0);

    for (int32_t i = 0; i < BODY_COUNT; ++i)
    {
        if (i == selected) wattron(win, A_REVERSE);

        mvwprintw(win, i + 1, 2, "%s", Kerbol[i].Name);

        if (i == selected) wattroff(win, A_REVERSE);
    }

    mvwprintw(win, 0, 2, " Bodies ");
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

void DrawResults(WINDOW *win, const Orbit *o1, const Orbit *res, UIState *UIState)
{
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Result ");

    if (UIState->resErr == RES_SUCCESS && UIState->orbitErr == ORBIT_SUCCESS && UIState->losErr == LOS_SUCCESS)
    {
        mvwprintw(win, 1, 2, "Resonant apoapsis:  %.2lf m", res->ApoapsisHeight(res));
        mvwprintw(win, 2, 2, "Resonant periapsis: %.2lf m", res->PeriapsisHeight(res));
        mvwprintw(win, 3, 2, "Eccentricity:       %lf", res->Eccentricity);

        mvwprintw(win, 5, 2, "Target Period:      %lf s", o1->OPeriod(o1));
        mvwprintw(win, 6, 2, "Resonant Period:    %lf s", res->OPeriod(res));
        mvwprintw(win, 7, 2, "DeltaV:             %.2f m/s",
                  DeltaVCircToEllip(o1, res, nullptr));
    }

    mvwprintw(win, 9, 2, "Resonant state: ");
    int32_t color = (UIState->resErr == RES_SUCCESS) ? COL_GREEN : COL_RED;
    wattron(win, COLOR_PAIR(color));
    wprintw(win, "%s", ErrToStr(UIState->resErr));
    wattroff(win, COLOR_PAIR(color));

    mvwprintw(win, 10, 2, "Orbit state:    ");
    color = (UIState->orbitErr == ORBIT_SUCCESS) ? COL_GREEN : COL_RED;
    wattron(win, COLOR_PAIR(color));
    wprintw(win, "%s", ErrToStr(UIState->orbitErr));
    wattroff(win, COLOR_PAIR(color));

    mvwprintw(win, 11, 2, "LOS state:      ");
    color = (UIState->losErr == LOS_SUCCESS) ? COL_GREEN : COL_RED;
    wattron(win, COLOR_PAIR(color));
    wprintw(win, "%s", ErrToStr(UIState->losErr));
    wattroff(win, COLOR_PAIR(color));

    wrefresh(win);
}

void DrawControls(WINDOW *win, f64_t altitude, const Orbit *o1, const UIState *UIState)
{

    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Input ");

    mvwprintw(win, 1, 2, "Altitude:   %.00f m", altitude);
    if (UIState->resErr == RES_SUCCESS && UIState->orbitErr == ORBIT_SUCCESS && UIState->losErr == LOS_SUCCESS)
    {
        mvwprintw(win, 2, 2, "Period:     %lf s", o1->OPeriod(o1));
    }
    else
    {
        mvwprintw(win, 2, 2, "Period:     0 s");
    }
    mvwprintw(win, 3, 2, "Satellites: %d", UIState->satelliteCount);
    mvwprintw(win, 4, 2, "Mode:       %s", ResonantModeToString(UIState->resMode));
    mvwprintw(win, 5, 2, "Precision:  %d m", UIState->precision);

    wrefresh(win);
}

void DrawFooter(WINDOW *win)
{
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "q: quit | up/down: body | left/right: altitude | m: mode | p: precision | w/s: satellite");

    wrefresh(win);
}

void changePrecision(UIState *UIState)
{
    switch (UIState->precision)
    {
        case 1000: UIState->precision = 100; break;
        case 100:  UIState->precision = 10; break;
        case 10:   UIState->precision = 1; break;
        case 1:    UIState->precision = 1000; break;
        default: break;
    }
}

int32_t main(void)
{
    initscr();
    start_color();
    init_pair(COL_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COL_RED, COLOR_RED,   COLOR_BLACK);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int32_t rows, cols;
    getmaxyx(stdscr, rows, (cols));

    constexpr auto footer_h = 3;
    const auto main_h   = rows - footer_h;

    const auto left_w  = cols / 2;
    const auto right_w = cols - left_w;

    const auto half_h  = main_h / 2;

    WINDOW *left_top    = newwin(half_h +5, left_w, 0, 0);
    WINDOW *left_bottom = newwin(main_h - half_h -5, left_w, half_h + 5, 0);

    WINDOW *right_top   = newwin(half_h, right_w, 0, left_w);
    WINDOW *right_bot   = newwin(main_h - half_h, right_w, half_h, left_w);

    WINDOW *footer      = newwin(footer_h, cols, main_h, 0);

    UIState state = {0};
    state.selected_body = MOHO;
    state.altitude = 150000;
    state.precision = 1000;
    state.satelliteCount = 3;
    state.resMode = MODE_PROGRADE;

    refresh();
    doupdate();

    bool running = true;
    while (running)
    {
        ResonantFunc func = GetResonantFunc(state.resMode);
        Orbit insertOrbit;
        Orbit targetOrbit = CreateOrbitCircularAlt(
            &Kerbol[state.selected_body],
            state.altitude,
            &state.orbitErr
        );

        if (state.orbitErr == ORBIT_SUCCESS && state.resErr == RES_SUCCESS && state.losErr == LOS_SUCCESS)
        {
            insertOrbit = func(
                &targetOrbit,
                state.satelliteCount,
                &state.resErr
            );
        }

        if (!LineofSight(&targetOrbit, state.satelliteCount, &state.losErr))
        {
            state.losErr = LOS_ERR_OCCLUDED_BY_SURFACE;
        }

        if (!AtmosphericOccusion(&targetOrbit, state.satelliteCount, &state.losErr))
        {
            state.losErr = LOS_ERR_OCCLUDED_BY_ATMOSPHERE;
        }

        DrawBodyList(left_top, state.selected_body);
        DrawBodyInfo(left_bottom, state.selected_body);
        DrawResults(right_top, &targetOrbit, &insertOrbit, &state);
        DrawControls(right_bot, state.altitude, &targetOrbit, &state);
        DrawFooter(footer);

        state.orbitErr = ORBIT_SUCCESS;
        state.resErr = RES_SUCCESS;
        state.losErr = LOS_SUCCESS;

        int32_t ch = getch();

        switch (ch)
        {
            case 'q':
                running = false;
                break;

            case 'p':
                changePrecision(&state);
            break;

            case 'm':
                state.resMode = (state.resMode + 1) % 2;
            break;

            case 'w':
                state.satelliteCount++;
            break;

            case 's':
                if (state.satelliteCount > 0) state.satelliteCount--;
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
                state.altitude += state.precision;
                break;

            case KEY_LEFT:
                state.altitude -= state.precision;
                if (state.altitude < state.precision) state.altitude = state.precision;
                break;
            default: break;
        }
    }

    endwin();
    return 0;
}