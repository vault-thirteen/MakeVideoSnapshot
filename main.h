#ifndef MAKEVIDEOSNAPSHOT_MAIN_H
#define MAKEVIDEOSNAPSHOT_MAIN_H

#define APP_EXIT_CODE_ALL_CLEAR 0
#define APP_EXIT_CODE_ARGS_COUNT_IS_BAD 1
#define APP_EXIT_CODE_ARG_PIXEL_FORMAT_IS_BAD 2
#define APP_EXIT_CODE_SNAPSHOT_MAKER_ERROR 3

#define APP_ARGS_COUNT 4

/*
 * Make snapshots of the video.
 *
 * @param argc      number of command line arguments
 * @param argv      textual values of command line arguments
 *
 * @return          positive error code in case of failure, otherwise 0.
 */
errno_t main(int argc, char **argv);

#endif //MAKEVIDEOSNAPSHOT_MAIN_H
