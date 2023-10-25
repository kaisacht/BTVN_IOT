import argparse
def args_parser():
    parser = argparse.ArgumentParser()
    # save file 
    parser.add_argument('--option', type=str, default='publish',
                        help="")
    args = parser.parse_args()
    return args