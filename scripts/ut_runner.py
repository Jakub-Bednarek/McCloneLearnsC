import argparse
import os
import pathlib
import subprocess

GREEN = "\033[92m"
RED = "\033[91m"
WHITE = "\033[97m"

TEXT_OFFSET = 3
NO_EXECUTABLES_TEXT = "NO EXECUTABLES FOUND"
BOX_EDGE_WIDTH = 1

SUCCESS_BOX = f"[{GREEN}✓{WHITE}]"
FAILURE_BOX = f"[{RED}X{WHITE}]"

SUMMARY_MESSAGE = f"{'*' * 10} {GREEN}SUMMARY{WHITE} {'*' * 10}"
ALL_TESTS_PASSED = f"{GREEN}ALL TESTS PASSED{WHITE}"


def collect_all_tests_executables(tests_dir):
    executables = []
    for path in os.listdir(tests_dir):
        file_path = os.path.join(tests_dir, path)
        if os.path.isfile(file_path):
            executables.append(file_path)
    return executables


def print_start_info(number_of_executables):
    if number_of_executables == 0:
        message_box_width = (
            2 * BOX_EDGE_WIDTH + 2 * TEXT_OFFSET + len(NO_EXECUTABLES_TEXT)
        )
        print("*" * message_box_width)
        print(
            f"*{' ' * TEXT_OFFSET}{RED}{NO_EXECUTABLES_TEXT}{WHITE}{' ' * TEXT_OFFSET}*"
        )
        print("*" * message_box_width)
        return
    else:
        message = f"*{' ' * TEXT_OFFSET}RUNNING {GREEN}{number_of_executables}{WHITE} TEST CASE{'S' if number_of_executables > 1 else ''}{' ' * TEXT_OFFSET}*"
        message_box_width = len(message) - (2 * len(GREEN))
        print("*" * message_box_width)
        print(message)
        print("*" * message_box_width)

    print("\n")


def print_summary(failed_execs, total_tests):
    print(f"{SUMMARY_MESSAGE}")
    if len(failed_execs) > 0:
        print("\n")

    for exec in failed_execs:
        print(f"{os.path.basename(exec)} - {RED}FAILURE{WHITE}")

    box_width = len(SUMMARY_MESSAGE) - len(GREEN) - len(WHITE)
    total_message = ""
    if len(failed_execs) > 0:
        total_message = f"{RED}{len(failed_execs)}/{total_tests} FAILED{WHITE}"
    else:
        total_message = ALL_TESTS_PASSED

    empty_space = box_width - len(total_message) + len(RED) + len(WHITE)
    side_space = int(empty_space / 2)
    print(f"\n{'*' * (side_space - 1)} {total_message} {'*' * (side_space - 1)}")


def run_test_cases(executables, stop_on_failure=False, std_output=False):
    number_of_testcases = len(executables)
    current_testcase = 1
    failed_testcase_names = []

    fnull = open(os.devnull, "w")
    for exec in executables:
        if std_output:
            process = subprocess.run(exec)
        else:
            process = subprocess.run(exec, stdout=fnull)

        print(
            f"{SUCCESS_BOX if not process.returncode else FAILURE_BOX} {os.path.basename(exec)} [{current_testcase}/{number_of_testcases}]"
        )
        if process.returncode:
            failed_testcase_names.append(exec)

            if stop_on_failure:
                break

        current_testcase += 1

    return failed_testcase_names


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--dir", help="Directory of test binaries", required=True)
    parser.add_argument("-f", "--fail", help="Stop on failed test", action="store_true")
    parser.add_argument(
        "-o", "--std-out", help="Log std output to console", action="store_true"
    )

    args = parser.parse_args()
    abs_path = pathlib.Path(args.dir).absolute()

    if not os.path.isdir(abs_path):
        print(f"{RED}Provided UT directory doesn't exists{WHITE}")
        exit(1)

    all_tests_executables = collect_all_tests_executables(abs_path)
    print_start_info(len(all_tests_executables))

    failed_execs = run_test_cases(
        all_tests_executables, stop_on_failure=args.fail, std_output=args.std_out
    )
    print("\n")

    print_summary(failed_execs, len(all_tests_executables))


if __name__ == "__main__":
    main()
