/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_get_next_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhyokki <jhyokki@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 15:33:49 by jhyokki           #+#    #+#             */
/*   Updated: 2024/12/12 21:07:22 by jhyokki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/* clang -Wall -Wextra -Werror -D BUFFER_SIZE=42 -o test_get_next_line \
test/test_get_next_line.c get_next_line.c get_next_line_utils.c \
$(pkg-config --cflags --libs check)
 */
#include <check.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "../get_next_line.h"

START_TEST(test_multiple_lines)
{
	int	fd = open("test/test_files/multiple_lines.txt", O_RDONLY);
	ck_assert_int_ne(fd, -1);

	char	*line;
	line = get_next_line(fd);
	ck_assert_str_eq(line, "First line\n");
	
	line = get_next_line(fd);
	ck_assert_str_eq(line, "Second line\n");
	
	line = get_next_line(fd);
	ck_assert_str_eq(line, "Third line\n");
	
	line = get_next_line(fd);
	ck_assert_ptr_null(line);

	close(fd);
}
END_TEST

START_TEST(test_empty_file)
{
	int	fd = open("test/test_files/empty.txt", O_RDONLY);
	ck_assert_int_ne(fd, -1);

	char	*line = get_next_line(fd);
	ck_assert_ptr_null(line);

	close(fd);
}
END_TEST

START_TEST(test_single_line)
{
	int	fd = open("test/test_files/single_line.txt", O_RDONLY);
	ck_assert_int_ne(fd, -1);

	char	*line;
	line = get_next_line(fd);
	ck_assert_str_eq(line, "Only one line\n");
	free(line);

	line = get_next_line(fd);
	ck_assert_ptr_null(line);

	close(fd);
}
END_TEST

START_TEST(test_invalid_fd)
{
	char	*line = get_next_line(-1);
	ck_assert_ptr_null(line);
}
END_TEST

Suite *get_next_line_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("GetNextLine");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_multiple_lines);
	tcase_add_test(tc_core, test_empty_file);
	tcase_add_test(tc_core, test_single_line);
	tcase_add_test(tc_core, test_invalid_fd);

	suite_add_tcase(s, tc_core);

	return (s);
}

int main(void)
{
	int		number_failed;
	Suite	*s;
	SRunner *sr;

	s = get_next_line_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);

	srunner_free(sr);

	return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
