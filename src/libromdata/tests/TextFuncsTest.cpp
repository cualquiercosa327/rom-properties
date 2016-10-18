/***************************************************************************
 * ROM Properties Page shell extension. (libromdata/tests)                 *
 * TextFuncsTest.cpp: TextFuncs class test.                                *
 *                                                                         *
 * Copyright (c) 2016 by David Korth.                                      *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; either version 2 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.           *
 ***************************************************************************/

// Google Test
#include "gtest/gtest.h"

// TextFuncs
#include "../TextFuncs.hpp"

// C includes. (C++ namespace)
#include <cstdio>

// C++ includes.
#include <string>
using std::string;
using std::u16string;

// NOTE: We're redefining ARRAY_SIZE() here in order to
// get a size_t instead of an int.
#ifdef ARRAY_SIZE
#unfed ARRAY_SIZE
#endif

/**
 * Number of elements in an array.
 *
 * Includes a static check for pointers to make sure
 * a dynamically-allocated array wasn't specified.
 * Reference: http://stackoverflow.com/questions/8018843/macro-definition-array-size
 */
#define ARRAY_SIZE(x) \
	(((sizeof(x) / sizeof(x[0]))) / \
		(size_t)(!(sizeof(x) % sizeof(x[0]))))

namespace LibRomData { namespace Tests {

class TextFuncsTest : public ::testing::Test
{
	protected:
		TextFuncsTest() { }

	public:
		// NOTE: Test strings are uint8_t[] in order to prevent
		// narrowing conversion warnings from appearing.

		/**
		 * cp1252 test string.
		 * Contains all possible cp1252 characters.
		 */
		static const uint8_t cp1252_data[250];

		/**
		 * cp1252 to UTF-8 test string.
		 * Contains the expected result from:
		 * - cp1252_to_utf8(cp1252_data, ARRAY_SIZE(cp1252_data))
		 * - cp1252_sjis_to_utf8(cp1252_data, ARRAY_SIZE(cp1252_data))
		 */
		static const uint8_t cp1252_utf8_data[388];

		/**
		 * cp1252 to UTF-16 test string.
		 * Contains the expected result from:
		 * - cp1252_to_utf16(cp1252_data, sizeof(cp1252_data))
		 * - cp1252_sjis_to_utf16(cp1252_data, sizeof(cp1252_data))
		 */
		static const uint16_t cp1252_utf16_data[250];

		/**
		 * Shift-JIS test string.
		 *
		 * TODO: Get a longer test string.
		 * This string is from the JP Pokemon Colosseum (GCN) save file.
		 */
		static const uint8_t sjis_data[34];

		/**
		 * Shift-JIS to UTF-8 test string.
		 * Contains the expected result from:
		 * - cp1252_sjis_to_utf8(sjis_data, ARRAY_SIZE(sjis_data))
		 */
		static const uint8_t sjis_utf8_data[50];

		/**
		 * Shift-JIS to UTF-16 test string.
		 * Contains the expected result from:
		 * - cp1252_sjis_to_utf16(sjis_data, ARRAY_SIZE(sjis_data))
		 */
		static const uint16_t sjis_utf16_data[18];
};

// Test strings are located in TextFuncsTest_data.hpp.
#include "TextFuncsTest_data.hpp"

/**
 * Test cp1252_to_utf8().
 */
TEST_F(TextFuncsTest, cp1252_to_utf8)
{
	// Test with implicit length.
	string str = cp1252_to_utf8((const char*)cp1252_data, -1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf8_data)-1);
	EXPECT_EQ((const char*)cp1252_utf8_data, str);

	// Test with explicit length.
	str = cp1252_to_utf8((const char*)cp1252_data, ARRAY_SIZE(cp1252_data)-1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf8_data)-1);
	EXPECT_EQ((const char*)cp1252_utf8_data, str);

	// Test with explicit length and trimmed NULLs.
	str = cp1252_to_utf8((const char*)cp1252_data, ARRAY_SIZE(cp1252_data));
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf8_data)-1);
	EXPECT_EQ((const char*)cp1252_utf8_data, str);
}

/**
 * Test cp1252_to_utf16().
 */
TEST_F(TextFuncsTest, cp1252_to_utf16)
{
	// Test with implicit length.
	u16string str = cp1252_to_utf16((const char*)cp1252_data, -1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf16_data)-1);
	EXPECT_EQ((const char16_t*)cp1252_utf16_data, str);

	// Test with explicit length.
	str = cp1252_to_utf16((const char*)cp1252_data, ARRAY_SIZE(cp1252_data)-1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf16_data)-1);
	EXPECT_EQ((const char16_t*)cp1252_utf16_data, str);

	// Test with explicit length and trimmed NULLs.
	str = cp1252_to_utf16((const char*)cp1252_data, ARRAY_SIZE(cp1252_data));
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf16_data)-1);
	EXPECT_EQ((const char16_t*)cp1252_utf16_data, str);
}

/**
 * Test cp1252_sjis_to_utf8() fallback functionality.
 * These strings should be detected as cp1252 due to
 * Shift-JIS decoding errors.
 */
TEST_F(TextFuncsTest, cp1252_sjis_to_utf8_fallback)
{
	// Test with implicit length.
	string str = cp1252_sjis_to_utf8((const char*)cp1252_data, -1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf8_data)-1);
	EXPECT_EQ((const char*)cp1252_utf8_data, str);

	// Test with explicit length.
	str = cp1252_sjis_to_utf8((const char*)cp1252_data, ARRAY_SIZE(cp1252_data)-1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf8_data)-1);
	EXPECT_EQ((const char*)cp1252_utf8_data, str);

	// Test with explicit length and trimmed NULLs.
	str = cp1252_sjis_to_utf8((const char*)cp1252_data, ARRAY_SIZE(cp1252_data));
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf8_data)-1);
	EXPECT_EQ((const char*)cp1252_utf8_data, str);
}

/**
 * Test cp1252_sjis_to_utf8() with ASCII strings.
 * Note that backslashes will *not* be converted to
 * yen symbols, so this should be a no-op.
 *
 * FIXME: Backslash may be converted to yen symbols
 * on Windows if the system has a Japanese locale.
 */
TEST_F(TextFuncsTest, cp1252_sjis_to_utf8_ascii)
{
	static const char cp1252_in[] = "C:\\Windows\\System32";

	// Test with implicit length.
	string str = cp1252_sjis_to_utf8(cp1252_in, -1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_in)-1);
	EXPECT_EQ(cp1252_in, str);

	// Test with explicit length.
	str = cp1252_sjis_to_utf8(cp1252_in, ARRAY_SIZE(cp1252_in)-1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_in)-1);
	EXPECT_EQ(cp1252_in, str);

	// Test with explicit length and trimmed NULLs.
	str = cp1252_sjis_to_utf8(cp1252_in, ARRAY_SIZE(cp1252_in));
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_in)-1);
	EXPECT_EQ(cp1252_in, str);
}

/**
 * Test cp1252_sjis_to_utf8() with Japanese text.
 */
TEST_F(TextFuncsTest, cp1252_sjis_to_utf8_japanese)
{
	// Test with implicit length.
	string str = cp1252_sjis_to_utf8((const char*)sjis_data, -1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(sjis_utf8_data)-1);
	EXPECT_EQ((const char*)sjis_utf8_data, str);

	// Test with explicit length.
	str = cp1252_sjis_to_utf8((const char*)sjis_data, ARRAY_SIZE(sjis_data)-1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(sjis_utf8_data)-1);
	EXPECT_EQ((const char*)sjis_utf8_data, str);

	// Test with explicit length and trimmed NULLs.
	str = cp1252_sjis_to_utf8((const char*)sjis_data, ARRAY_SIZE(sjis_data));
	EXPECT_EQ(str.size(), ARRAY_SIZE(sjis_utf8_data)-1);
	EXPECT_EQ((const char*)sjis_utf8_data, str);
}

/**
 * Test cp1252_sjis_to_utf16() fallback functionality.
 * These strings should be detected as cp1252 due to
 * Shift-JIS decoding errors.
 */
TEST_F(TextFuncsTest, cp1252_sjis_to_utf16_fallback)
{
	// Test with implicit length.
	u16string str = cp1252_sjis_to_utf16((const char*)cp1252_data, -1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf16_data)-1);
	EXPECT_EQ((const char16_t*)cp1252_utf16_data, str);

	// Test with explicit length.
	str = cp1252_sjis_to_utf16((const char*)cp1252_data, ARRAY_SIZE(cp1252_data)-1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf16_data)-1);
	EXPECT_EQ((const char16_t*)cp1252_utf16_data, str);

	// Test with explicit length and trimmed NULLs.
	str = cp1252_sjis_to_utf16((const char*)cp1252_data, ARRAY_SIZE(cp1252_data));
	EXPECT_EQ(str.size(), ARRAY_SIZE(cp1252_utf16_data)-1);
	EXPECT_EQ((const char16_t*)cp1252_utf16_data, str);
}

/**
 * Test cp1252_sjis_to_utf16() with ASCII strings.
 * Note that backslashes will *not* be converted to
 * yen symbols, so this should be a no-op.
 *
 * FIXME: Backslash may be converted to yen symbols
 * on Windows if the system has a Japanese locale.
 */
TEST_F(TextFuncsTest, cp1252_sjis_to_utf16_ascii)
{
	static const char cp1252_in[] = "C:\\Windows\\System32";
	// TODO: Hex and/or _RP_CHR()?
	static const char16_t u16_out[] = {
		'C',':','\\','W','i','n','d','o',
		'w','s','\\','S','y','s','t','e',
		'm','3','2',0
	};

	// Test with implicit length.
	u16string str = cp1252_sjis_to_utf16(cp1252_in, -1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(u16_out)-1);
	EXPECT_EQ(u16_out, str);

	// Test with explicit length.
	str = cp1252_sjis_to_utf16(cp1252_in, ARRAY_SIZE(cp1252_in)-1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(u16_out)-1);
	EXPECT_EQ(u16_out, str);

	// Test with explicit length and trimmed NULLs.
	str = cp1252_sjis_to_utf16(cp1252_in, ARRAY_SIZE(cp1252_in));
	EXPECT_EQ(str.size(), ARRAY_SIZE(u16_out)-1);
	EXPECT_EQ(u16_out, str);
}

/**
 * Test cp1252_sjis_to_utf16() with Japanese text.
 */
TEST_F(TextFuncsTest, cp1252_sjis_to_utf16_japanese)
{
	// Test with implicit length.
	u16string str = cp1252_sjis_to_utf16((const char*)sjis_data, -1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(sjis_utf16_data)-1);
	EXPECT_EQ((const char16_t*)sjis_utf16_data, str);

	// Test with explicit length.
	str = cp1252_sjis_to_utf16((const char*)sjis_data, ARRAY_SIZE(sjis_data)-1);
	EXPECT_EQ(str.size(), ARRAY_SIZE(sjis_utf16_data)-1);
	EXPECT_EQ((const char16_t*)sjis_utf16_data, str);

	// Test with explicit length and trimmed NULLs.
	str = cp1252_sjis_to_utf16((const char*)sjis_data, ARRAY_SIZE(sjis_data));
	EXPECT_EQ(str.size(), ARRAY_SIZE(sjis_utf16_data)-1);
	EXPECT_EQ((const char16_t*)sjis_utf16_data, str);
}

} }

/**
 * Test suite main function.
 */
extern "C" int gtest_main(int argc, char *argv[])
{
	fprintf(stderr, "LibRomData test suite: TextFuncs tests.\n\n");
	fflush(nullptr);

	// coverity[fun_call_w_exception]: uncaught exceptions cause nonzero exit anyway, so don't warn.
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
