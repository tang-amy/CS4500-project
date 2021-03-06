#include "../src/dataframe/dataframe.h"
#include <assert.h>

/********************************************************************************************************************
 * testing FastArray
 * 
 **/

/**
 * @brief Tests the get() function in all FastArrays
 * 
 */
void testGet()
{
    FastArray<int> *i = new FastArray<int>();
    FastArray<float> *f = new FastArray<float>();
    for (int i_ = 1; i_ < 6; i_++)
    {
        i->push_back(i_);
        f->push_back(4.20);
    }

    FastArray<bool> *b = new FastArray<bool>();
    b->push_back(true);
    b->push_back(false);

    FastArray<String *> *s = new FastArray<String *>();
    s->push_back(new String("hello"));
    s->push_back(new String("world"));

    assert(i->get(0) == 1);
    assert(i->get(4) == 5);
    assert(b->get(0) == true);
    assert(b->get(1) == false);
    assert(f->get(0) - 4.20 < 0.001 && f->get(0) - 4.20 > -.001);
    assert(f->get(3) - 4.20 < 0.001 && f->get(3) - 4.20 > -.001);
    assert(s->get(0)->equals(new String("hello")));
    assert(s->get(1)->equals(new String("world")));
}

/**
 * @brief Tests the set() function in all FastArrays
 * 
 */
void testSet()
{
    FastArray<int> *i = new FastArray<int>();
    FastArray<float> *f = new FastArray<float>();
    for (int i_ = 1; i_ < 6; i_++)
    {
        i->push_back(i_);
        f->push_back(4.20);
    }

    FastArray<bool> *b = new FastArray<bool>();
    b->push_back(true);
    b->push_back(false);

    FastArray<String *> *s = new FastArray<String *>();
    s->push_back(new String("hello"));
    s->push_back(new String("world"));

    i->set(0, 420);
    i->set(4, 69);
    assert(i->get(0) == 420);
    assert(i->get(4) == 69);

    b->set(0, true);
    b->set(1, false);
    assert(b->get(0) == true);
    assert(b->get(1) == false);

    f->set(0, 4.20);
    f->set(3, 6.9);
    assert(f->get(0) - 4.20 < 0.001 && f->get(0) - 4.20 > -.001);
    assert(f->get(3) - 6.9 < 0.001 && f->get(3) - 6.9 > -.001);

    String *h = new String("Heeellloooooo"); // no need to delete these as FastArrays own their contents
    String *w = new String("wooooorllddddd");
    s->set(0, h);
    s->set(1, w);
    assert(s->get(0)->equals(h));
    assert(s->get(1)->equals(w));
}

/**
 * @brief Tests the grow() function in all FastArrays by
 * adding 512 elements to each, which would grow
 * the outer array twice and the inner array(s) multiple
 * times
 * 
 * Also tests push_back() because we have to use that
 * function to test grow()
 * 
 */
void testGrowAndPushBack()
{
    FastArray<int> *i = new FastArray<int>();
    FastArray<float> *f = new FastArray<float>();
    for (int i_ = 1; i_ < 6; i_++)
    {
        i->push_back(i_);
        f->push_back(4.20);
    }

    FastArray<bool> *b = new FastArray<bool>();
    b->push_back(true);
    b->push_back(false);

    FastArray<String *> *s = new FastArray<String *>();
    s->push_back(new String("hello"));
    s->push_back(new String("world"));

    for (int j = 0; j < 512; j++)
    {
        i->push_back(j);
        assert(i->get(j + 5) == j);
    }

    for (int j = 0; j < 512; j++)
    {
        b->push_back(0);
        assert(b->get(j + 2) == 0);
    }

    for (int j = 0; j < 512; j++)
    {
        f->push_back(4.20);
        assert(f->get(j + 5) - 4.20 < 0.001 && f->get(j + 5) - 4.20 > -.001);
    }

    String *str = new String("hello world"); // FastArrays own their contents, so no need to delete this at the end
    for (int j = 0; j < 512; j++)
    {
        s->push_back(str);
        assert(s->get(j + 2)->equals(str));
    }
}

/********************************************************************************************************************
 * testing Schema
 * 
**/

/**
 * @brief Testing adding a Column with a name to a nonempty schema via checking that col_types is as expected and the 
 * number of columns is the expected number.
 * 
 */
void testAddColumn()
{
    String *s_str = new String("FIBS");
    Schema *s = new Schema(s_str->c_str());

    assert(s->coltypes_->equals(s_str));
    s->add_column('S');
    String *s_str_new = new String("FIBSS");

    assert(s->width() == 5);
    assert(s->coltypes_->equals(s_str_new));
}

/**
 * @brief Testing adding a Column without a name to a nonempty schema via checking that col_types is as expected and the 
 * number of columns is the expected number.
 * 
 */
void testAddEmptyNameColumn()
{
    String *s_str = new String("FIBS");
    Schema *s = new Schema(s_str->c_str());

    assert(s->coltypes_->equals(s_str));
    s->add_column('S');
    String *s_str_new = new String("FIBSS");

    assert(s->width() == 5);
    assert(s->coltypes_->equals(s_str_new));
}

/**
 * @brief Testing adding a Column without a name to a empty schema via checking that col_types is as expected and the 
 * number of columns is the expected number.
 * 
 */
void testAddColToEmptySchemaNoName()
{
    Schema *s = new Schema();

    s->add_column('S');

    assert(s->width() == 1);
    assert(s->coltypes_->equals(new String("S")));
}

/**
 * @brief Testing adding a Column with a name to a empty schema via checking that col_types is as expected and the 
 * number of columns is the expected number.
 * 
 */
void testAddColToEmptySchema()
{
    Schema *s = new Schema();

    s->add_column('S');

    assert(s->width() == 1);
    assert(s->coltypes_->equals(new String("S")));
}

/********************************************************************************************************************
 * testing an implementation Rower iterating over Rows
 * 
 **/

/**
 * @brief tests Rower's accept() function on rows populated via a nonempty dataframe/schema.
 * 
 */
void testAcceptNonemptyRow()
{
    Schema *s = new Schema("IISFB");    // nonempty schema
    DataFrame *df = new DataFrame(*s);  // make df based on nonempty schema
    Row *r = new Row(df->get_schema()); // making a row based off the schema
    assert(true);
    // filling df with rows
    for (size_t i = 0; i < 1000; i++)
    {
        r->set(0, (int)i);
        r->set(1, (int)i + 1);
        r->set(2, new String("string hello"));
        r->set(3, (float)(i + 0.22));
        r->set(4, i % 2 == 1);
        df->add_row(*r);
    }
    delete r;
    RowPrinter printer_ = RowPrinter(); // creating rower object
    // iterating through the rows and checking that the rower accepts each row given to it
    Row *r_ = new Row(df->get_schema());
    for (size_t i = 0; i < df->nrows(); i++)
    {
        r_ = new Row(df->get_schema());
        df->fill_row(i, *r_);
        assert(printer_.accept(*r_));
        delete r_;
    }
}

/**
 * @brief tests Rower's accept() function on a row created via an empty Schema
 * 
 */
void testAcceptEmptyRow()
{
    Schema *s = new Schema(); // empty schema
    Row r = Row(*s);          // making a row from empty schema
    RowPrinter rower_ = RowPrinter();
    assert(rower_.accept(r));
    delete s;
}

/********************************************************************************************************************
 * testing Columns
 * 
 **/

/**
 * @brief Checking to see if we can accurately get the elements from an IntColumn
 * 
 */
void testIntColumnGet1()
{
    IntColumn *i = new IntColumn();
    for (int j = 1; j < 18; j++)
        i->push_back(j);
    for (int j = 0; j < 17; j++)
        assert(i->get(j) == j + 1);
    delete i;
}

/**
 * @brief Testing set function via setting the values then getting them to check if we set the value correctly as each location.
 * 
 */
void testIntColumnSet2()
{
    IntColumn *i = new IntColumn();
    for (int j = 1; j < 18; j++)
    {
        i->push_back(j);
    }
    // set 1 value in i to something else and check if that one value changed correctly
    i->set(8, 8888);
    assert(i->get(8) == 8888);
    // set all values in i to something else and check
    for (int j = 0; j < 17; j++)
    {
        i->set(j, j * 3);
    }
    for (size_t j = 0; j < 17; j++)
    {
        assert(i->get(j) == j * 3);
    }
    delete i;
}

/**
 * @brief Testing that push_back() function works as expected for empty and nonempty Columns. Check after each push back that the added element is
 * where and what is expected.
 * 
 */
void testIntColumnPushBack3()
{
    // checking empty column
    IntColumn *i_ = new IntColumn();
    // adding to empty column and checking to see if added element is where and what we expect
    for (size_t j = 0; j < 10000000; j++)
    {
        i_->push_back(j);
        assert(i_->get(j) == j);
    }
    delete i_;

    // checking nonempty array
    IntColumn *i = new IntColumn();
    for (int j = 1; j < 18; j++)
    {
        i->push_back(j);
    }
    for (size_t j = 0; j < 10000000; j++)
    {
        i->push_back(j * 3);
        assert(i->get(j + 17) == (j * 3));
    }
    delete i;
}

/**
 * @brief Testing that size works as expected for empty and nonempty Columns including after we add new elements via push_back() function
 * 
 */
void testIntColumnSize4()
{
    // checking empty column
    IntColumn *i_ = new IntColumn();
    assert(i_->size() == 0);
    // adding to empty column and checking to see if size is as expected after each add and at the end
    for (size_t j = 0; j < 10000000; j++)
    {
        i_->push_back(j);
        assert(i_->size() == (j + 1));
    }
    assert(i_->size() == 10000000);
    delete i_;
    // checking nonempty column
    IntColumn *i = new IntColumn();
    for (int j = 1; j < 18; j++)
    {
        i->push_back(j);
    }
    assert(i->size() == 17);
    // adding to nonempty column and checking to see if size is as expected after each add and at the end
    for (size_t j = 0; j < 10000000; j++)
    {
        i->push_back(j);
        assert(i->size() == (j + 18));
    }
    for (size_t j = 0; j < 17; j++)
    {
        assert(i->get(j) == j + 1);
    }
    delete i;
}

/**
 * @brief Testing if empty/nonempty IntColumn returns the expected pointer
 * 
 */
void testIntColumnAsType5()
{
    // checking empty column
    IntColumn *i_ = new IntColumn();
    assert(i_ == i_->as_int());
    assert(i_->as_bool() == nullptr);
    assert(i_->as_float() == nullptr);
    assert(i_->as_string() == nullptr);
    // checking nonempty column
    IntColumn *i = new IntColumn();
    for (int j = 1; j < 18; j++)
    {
        i->push_back(j);
    }
    assert(i == i->as_int());
    assert(i->as_bool() == nullptr);
    assert(i->as_float() == nullptr);
    assert(i->as_string() == nullptr);
    delete i;
    delete i_;
}

/**
 * @brief Testing if empty/nonempty IntColumn returns the expected char representing its type
 * 
 */
void testIntColumnGetType6()
{
    // checking empty column
    Column *i_ = new IntColumn();
    assert(i_->get_type() == 'I');
    // checking nonempty column
    IntColumn *i = new IntColumn();
    for (int j = 1; j < 18; j++)
    {
        i->push_back(j);
    }
    assert(i->get_type() == 'I');
    delete i;
    delete i_;
}

/**
 * @brief Tests the clone method for an IntColumn. Because Columns have pointer equality, to see if clone is correct, we test to see if each element is the same. 
 * Additionally, because a clone should not directly point at something the Column owns, the original Column is deleted and then the values in the clone are checked
 * again.
 * 
 */
void testIntColumnClone7()
{
    // create and populate IntCol
    IntColumn *i = new IntColumn();
    for (size_t j = 0; j < 10000; j++)
    {
        i->push_back(j);
    }
    IntColumn *i_ = i->clone();
    assert(i->size() == i_->size());
    // check manually if each element is the same
    for (size_t j = 0; j < i->size(); j++)
    {
        assert(i->get(j) == i_->get(j));
    }
    delete i;
    // check that the clone retains information/elements after original is deleted
    for (size_t j = 0; j < 10000; j++)
    {
        assert(i_->get(j) == j);
    }
    delete i_;
}

/**
 * @brief Checking to see if we can accurately get the elements from an BoolColumn
 * 
 */
void testBoolColumnGet1()
{
    BoolColumn *i = new BoolColumn();
    for (int j = 0; j < 18; j++)
    {
        i->push_back(j % 2 == 0);
    }
    for (size_t j = 0; j < 18; j++)
    {
        assert(i->get(j) == (j % 2 == 0));
    }
    delete i;
}

/**
 * @brief Testing set function via setting the values then getting them to check if we set the value correctly as each location.
 * 
 */
void testBoolColumnSet2()
{
    BoolColumn *i = new BoolColumn();
    for (int j = 0; j < 18; j++)
    {
        i->push_back(j % 2 == 0);
    }
    // set 1 value in i to something else and check if that one value changed correctly
    i->set(8, false);
    assert(!i->get(8));
    // set all values in i to something else and check
    for (size_t j = 0; j < 18; j++)
    {
        i->set(j, (j % 2 == 1));
    }
    for (size_t j = 0; j < 18; j++)
    {
        bool expected = (j % 2 == 1);
        assert(i->get(j) == expected);
    }
    delete i;
}

/**
 * @brief Testing that push_back() function works as expected for empty and nonempty Columns. Check after each push back that the added element is
 * where and what is expected.
 * 
 */
void testBoolColumnPushBack3()
{
    // checking empty column
    BoolColumn *i_ = new BoolColumn();
    // adding to empty column and checking to see if added element is where and what we expect
    for (size_t j = 0; j < 10000000; j++)
    {
        i_->push_back(j % 2 == 0);
        assert(i_->get(j) == (j % 2 == 0));
    }
    delete i_;

    // checking nonempty array
    BoolColumn *i = new BoolColumn();
    for (int j = 0; j < 18; j++)
    {
        i->push_back(j % 2 == 0);
    }
    for (size_t j = 0; j < 10000000; j++)
    {
        i->push_back((j % 2 == 1));
        assert(i->get(j + 18) == (j % 2 == 1));
    }
    // check that the beginning is unchanged by pushback
    for (size_t j = 0; j < 18; j++)
    {
        assert(i->get(j) == (j % 2 == 0));
    }
    delete i;
}

/**
 * @brief Testing that size works as expected for empty and nonempty Columns including after we add new elements via push_back() function
 * 
 */
void testBoolColumnSize4()
{
    // checking empty column
    BoolColumn *i_ = new BoolColumn();
    assert(i_->size() == 0);
    // adding to empty column and checking to see if size is as expected after each add and at the end
    for (size_t j = 0; j < 10000000; j++)
    {
        i_->push_back(true);
        assert(i_->size() == (j + 1));
    }
    assert(i_->size() == 10000000);
    delete i_;
    // checking nonempty column
    BoolColumn *i = new BoolColumn();
    for (int j = 0; j < 18; j++)
    {
        i->push_back(j % 2 == 0);
    }
    assert(i->size() == 18);
    // adding to nonempty column and checking to see if size is as expected after each add and at the end
    for (size_t j = 0; j < 10000000; j++)
    {
        i->push_back(true);
        assert(i->size() == (j + 19));
    }
    //assert(i->size() == 10000018);
    delete i;
}

/**
 * @brief Testing if empty/nonempty BoolColumn returns the expected poBooler
 * 
 */
void testBoolColumnAsType5()
{
    // checking empty column
    BoolColumn *i_ = new BoolColumn();
    assert(i_ == i_->as_bool());
    assert(i_->as_int() == nullptr);
    assert(i_->as_float() == nullptr);
    assert(i_->as_string() == nullptr);
    // checking nonempty column
    BoolColumn *i = new BoolColumn();
    for (int j = 0; j < 18; j++)
    {
        i->push_back(j % 2 == 0);
    }
    assert(i == i->as_bool());
    assert(i->as_int() == nullptr);
    assert(i->as_float() == nullptr);
    assert(i->as_string() == nullptr);
    delete i;
    delete i_;
}

/**
 * @brief Testing if empty/nonempty BoolColumn returns the expected char representing its type
 * 
 */
void testBoolColumnGetType6()
{
    // checking empty column
    Column *i_ = new BoolColumn();
    assert(i_->get_type() == 'B');
    // checking nonempty column
    BoolColumn *i = new BoolColumn();
    for (int j = 0; j < 18; j++)
    {
        i->push_back(j % 2 == 0);
    }
    assert(i->get_type() == 'B');
    delete i;
    delete i_;
}

/**
 * @brief Tests the clone method for an BoolColumn. Because Columns have pointer equality, to see if clone is correct, we test to see if each element is the same. 
 * Additionally, because a clone should not directly pointer at something the Column owns, the original Column is deleted and then the values in the clone are checked
 * again.
 * 
 */
void testBoolColumnClone7()
{
    // create and populate BoolCol
    BoolColumn *i = new BoolColumn();
    for (size_t j = 0; j < 10000; j++)
    {
        i->push_back((j % 2 == 1));
    }
    BoolColumn *i_ = i->clone();
    assert(i->size() == i_->size());
    // check manually if each element is the same
    for (size_t j = 0; j < i->size(); j++)
    {
        assert(i->get(j) == i_->get(j));
    }
    delete i;
    // check that the clone retains information/elements after original is deleted
    for (size_t j = 0; j < 10000; j++)
    {
        assert(i_->get(j) == (j % 2 == 1));
    }
    delete i_;
}

/**
 * @brief Checking to see if we can accurately get the elements from an FloatColumn
 * 
 */
void testFloatColumnGet1()
{
    FloatColumn *i = new FloatColumn();
    for (int j = 0; j < 17; j++)
    {
        i->push_back(float(j) + 1.22);
    }
    for (size_t j = 0; j < 17; j++)
    {
        float expected = float(j) + 1.22;
        assert((i->get(j) - expected < 0.001) && (i->get(j) - expected > -0.001));
    }
    delete i;
}

/**
 * @brief Testing set function via setting the values then getting them to check if we set the value correctly as each location.
 * 
 */
void testFloatColumnSet2()
{
    FloatColumn *i = new FloatColumn();
    for (int j = 0; j < 17; j++)
    {
        i->push_back(float(j) + 1.22);
    }
    // set 1 value in i to something else and check if that one value changed correctly
    i->set(8, 8.88);
    assert(i->get(8) - 8.88 < 0.001 && i->get(8) - 8.88 > -0.001);
    // set all values in i to something else and check
    for (size_t j = 0; j < 17; j++)
    {
        i->set(j, (float)j + 2.22);
    }
    for (size_t j = 0; j < 17; j++)
    {
        float expected = float(j) + 2.22;
        assert((i->get(j) - expected) < 0.001);
        assert((i->get(j) - expected) > -0.001);
    }
    delete i;
}

/**
 * @brief Testing that push_back() function works as expected for empty and nonempty Columns. Check after each push back that the added element is
 * where and what is expected.
 * 
 */
void testFloatColumnPushBack3()
{
    // checking empty column
    FloatColumn *i_ = new FloatColumn();
    // adding to empty column and checking to see if added element is where and what we expect
    for (size_t j = 0; j < 10000000; j++)
    {
        float expected = float(j) + 2.42;
        i_->push_back(expected);
        assert((i_->get(j) - expected) < 0.001);
        assert((i_->get(j) - expected) > -0.001);
    }
    delete i_;

    // checking nonempty array
    FloatColumn *i = new FloatColumn();
    for (int j = 0; j < 17; j++)
    {
        i->push_back(float(j) + 1.22);
    }
    for (size_t j = 0; j < 10000000; j++)
    {
        float expected = float(j) + 2.44;
        i->push_back(expected);
        assert((i->get(j + 17) - expected) < 0.001);
        assert((i->get(j + 17) - expected) > -0.001);
    }
    // check that the beginning is unchanged by pushback
    for (size_t j = 0; j < 17; j++)
    {
        float expected = float(j) + 1.22;
        assert((i->get(j) - expected) < 0.001);
        assert((i->get(j) - expected) > -0.001);
    }
    delete i;
}

/**
 * @brief Testing that size works as expected for empty and nonempty Columns including after we add new elements via push_back() function
 * 
 */
void testFloatColumnSize4()
{
    // checking empty column
    FloatColumn *i_ = new FloatColumn();
    assert(i_->size() == 0);
    // adding to empty column and checking to see if size is as expected after each add and at the end
    for (size_t j = 0; j < 10000000; j++)
    {
        i_->push_back(2.22);
        assert(i_->size() == (j + 1));
    }
    assert(i_->size() == 10000000);
    delete i_;
    // checking nonempty column
    FloatColumn *i = new FloatColumn();
    for (int j = 0; j < 17; j++)
    {
        i->push_back(float(j) + 1.22);
    }
    assert(i->size() == 17);
    // adding to nonempty column and checking to see if size is as expected after each add and at the end
    for (size_t j = 0; j < 10000000; j++)
    {
        i->push_back(2.22);
        assert(i->size() == (j + 18));
    }
    //assert(i->size() == 10000017);
    delete i;
}

/**
 * @brief Testing if empty/nonempty FloatColumn returns the expected poFloater
 * 
 */
void testFloatColumnAsType5()
{
    // checking empty column
    FloatColumn *i_ = new FloatColumn();
    assert(i_ == i_->as_float());
    assert(i_->as_bool() == nullptr);
    assert(i_->as_int() == nullptr);
    assert(i_->as_string() == nullptr);
    // checking nonempty column
    FloatColumn *i = new FloatColumn();
    for (int j = 0; j < 17; j++)
    {
        i->push_back(float(j) + 1.22);
    }
    assert(i == i->as_float());
    assert(i->as_bool() == nullptr);
    assert(i->as_int() == nullptr);
    assert(i->as_string() == nullptr);
    delete i;
    delete i_;
}

/**
 * @brief Testing if empty/nonempty FloatColumn returns the expected char representing its type
 * 
 */
void testFloatColumnGetType6()
{
    // checking empty column
    Column *i_ = new FloatColumn();
    assert(i_->get_type() == 'F');
    // checking nonempty column
    FloatColumn *i = new FloatColumn();
    for (int j = 0; j < 17; j++)
    {
        i->push_back(float(j) + 1.22);
    }
    assert(i->get_type() == 'F');
    delete i;
    delete i_;
}

/**
 * @brief Tests the clone method for an FloatColumn. Because Columns have pointer equality, to see if clone is correct, we test to see if each element is the same. 
 * Additionally, because a clone should not directly pointer at something the Column owns, the original Column is deleted and then the values in the clone are checked
 * again.
 * 
 */
void testFloatColumnClone7()
{
    // create and populate FloatCol
    FloatColumn *i = new FloatColumn();
    for (size_t j = 0; j < 10000; j++)
    {
        i->push_back(j + 2.345);
    }
    FloatColumn *i_ = i->clone();
    assert(i->size() == i_->size());
    // check manually if each element is the same
    for (size_t j = 0; j < i->size(); j++)
    {
        assert(i->get(j) - i_->get(j) < 0.001);
        assert(i->get(j) - i_->get(j) > -0.001);
    }
    delete i;
    // check that the clone retains information/elements after original is deleted
    for (size_t j = 0; j < 10000; j++)
    {
        float expected = j + 2.345;
        assert(i_->get(j) - expected < 0.001);
        assert(i_->get(j) - expected > -0.001);
    }
    delete i_;
}

/**
 * @brief Checking to see if we can accurately get the elements from an StringColumn
 * 
 */
void testStringColumnGet1()
{
    String *all = new String("helloworldrejects");
    StringColumn *i = new StringColumn();
    char *c_;
    String *toAdd;
    for (size_t j = 0; j < 17; j++)
    {
        c_ = new char(all->at(j));
        toAdd = new String(c_);
        i->push_back(toAdd->clone());
        delete toAdd;
        delete c_;
    }
    char *c;
    String *expected_;
    for (size_t j = 0; j < 17; j++)
    {
        c = new char(all->at(j));
        expected_ = new String(c);
        assert(i->get(j)->equals(expected_));
        delete expected_;
        delete c;
    }
    delete i;
    delete all;
}

/**
 * @brief Testing set function via setting the values then getting them to check if we set the value correctly as each location.
 * 
 */
void testStringColumnSet2()
{
    String *all = new String("helloworldrejects");
    String *new_ = new String("blah");
    String *all2 = new String("blahhityblahhityblah");
    StringColumn *i = new StringColumn();
    char *c_;
    String *toAdd;
    for (size_t j = 0; j < 17; j++)
    {
        c_ = new char(all->at(j));
        toAdd = new String(c_);
        i->push_back(toAdd->clone());
        delete toAdd;
        delete c_;
    }
    // set 1 value in i to something else and check if that one value changed correctly
    i->set(8, new String("blah"));
    assert(i->get(8)->equals(new_));
    // set all values in i to something else and check
    char *c;
    String *expected_;
    for (size_t j = 0; j < 17; j++)
    {
        c = new char(all2->at(j));
        expected_ = new String(c);
        i->set(j, expected_->clone());
        delete c;
        delete expected_;
    }
    for (size_t j = 0; j < 17; j++)
    {
        c = new char(all2->at(j));
        expected_ = new String(c);
        assert(i->get(j)->equals(expected_));
        delete c;
        delete expected_;
    }
    delete i;
    delete all;
    delete all2;
    delete new_;
}

/**
 * @brief Testing that push_back() function works as expected for empty and nonempty Columns. Check after each push back that the added element is
 * where and what is expected.
 * 
 */
void testStringColumnPushBack3()
{
    String *all = new String("blahhityblahhityblah");
    size_t size_ = all->size();
    // checking empty column
    StringColumn *i_ = new StringColumn();
    char *c;
    String *expected;
    // adding to empty column and checking to see if added element is where and what we expect
    for (size_t j = 0; j < 10000000; j++)
    {
        c = new char(all->at(j % size_));
        expected = new String(c);
        i_->push_back(expected->clone());
        assert(i_->get(j)->equals(expected));
        delete c;
        delete expected;
    }
    delete i_;
    // checking nonempty array
    String *all2 = new String("helloworldrejects");
    StringColumn *i = new StringColumn();
    char *c_;
    String *toAdd;
    for (size_t j = 0; j < 17; j++)
    {
        c_ = new char(all2->at(j));
        toAdd = new String(c_);
        i->push_back(toAdd->clone());
        delete toAdd;
        delete c_;
    }
    for (size_t j = 0; j < 10000; j++)
    {
        c = new char(all->at(j % size_));
        String *expected = new String(c);
        i->push_back(expected->clone());
        assert(i->size() == j + 18);
        assert(i->get(j + 17)->equals(expected));
    }
    // check that the beginning is unchanged by pushback
    for (size_t j = 0; j < 17; j++)
    {
        c = new char(all2->at(j));
        expected = new String(c);
        assert(i->get(j)->equals(expected));
        delete c;
        delete expected;
    }
    delete i;
    delete all;
    delete all2;
}

/**
 * @brief Testing that size works as expected for empty and nonempty Columns including after we add new elements via push_back() function
 * 
 */
void testStringColumnSize4()
{
    // checking empty column
    StringColumn *i_ = new StringColumn();
    assert(i_->size() == 0);
    // adding to empty column and checking to see if size is as expected after each add and at the end
    for (size_t j = 0; j < 10000000; j++)
    {
        i_->push_back(new String("h"));
        assert(i_->size() == (j + 1));
    }
    assert(i_->size() == 10000000);
    delete i_;
    // checking nonempty column
    String *all = new String("helloworldrejects");
    StringColumn *i = new StringColumn();
    char *c_;
    String *toAdd;
    for (size_t j = 0; j < 17; j++)
    {
        c_ = new char(all->at(j));
        toAdd = new String(c_);
        i->push_back(toAdd->clone());
        delete toAdd;
        delete c_;
    }
    assert(i->size() == 17);
    // adding to nonempty column and checking to see if size is as expected after each add and at the end
    for (size_t j = 0; j < 10000000; j++)
    {
        i->push_back(new String("h"));
        assert(i->size() == (j + 18));
    }
    assert(i->size() == 10000017);
    delete i;
}

/**
 * @brief Testing if empty/nonempty StringColumn returns the expected pointer
 * 
 */
void testStringColumnAsType5()
{
    // checking empty column
    StringColumn *i_ = new StringColumn();
    assert(i_ == i_->as_string());
    assert(i_->as_bool() == nullptr);
    assert(i_->as_int() == nullptr);
    assert(i_->as_float() == nullptr);
    // checking nonempty column
    String *all = new String("helloworldrejects");
    StringColumn *i = new StringColumn();
    char *c;
    String *toAdd;
    for (size_t j = 0; j < 17; j++)
    {
        c = new char(all->at(j));
        toAdd = new String(c);
        i->push_back(toAdd->clone());
        delete toAdd;
        delete c;
    }
    assert(i == i->as_string());
    assert(i->as_bool() == nullptr);
    assert(i->as_int() == nullptr);
    assert(i->as_float() == nullptr);
    delete i;
    delete i_;
}

/**
 * @brief Testing if empty/nonempty StringColumn returns the expected char representing its type
 * 
 */
void testStringColumnGetType6()
{
    // checking empty column
    Column *i_ = new StringColumn();
    assert(i_->get_type() == 'S');
    // checking nonempty column
    String *all = new String("helloworldrejects");
    StringColumn *i = new StringColumn();
    char *c;
    String *toAdd;
    for (size_t j = 0; j < 17; j++)
    {
        c = new char(all->at(j));
        toAdd = new String(c);
        i->push_back(toAdd->clone());
        delete toAdd;
        delete c;
    }
    assert(i->get_type() == 'S');
    delete i;
    delete i_;
}

/**
 * @brief Tests the clone method for an StringColumn. Because Columns have pointer equality, to see if clone is correct, we test to see if each element is the same. 
 * Additionally, because a clone should not directly pointer at something the Column owns, the original Column is deleted and then the values in the clone are checked
 * again.
 * 
 */
void testStringColumnClone7()
{
    // create and populate StringCol
    String *all = new String("blahhityblahhityblah");
    size_t size_ = all->size();
    StringColumn *i = new StringColumn();
    char *c;
    String *expected;
    for (size_t j = 0; j < 10000; j++)
    {
        c = new char(all->at(j % size_));
        expected = new String(c);
        i->push_back(expected->clone());
    }
    StringColumn *i_ = i->clone();
    assert(i->size() == i_->size());
    // check manually if each element is the same
    for (size_t j = 0; j < i->size(); j++)
    {
        assert(i->get(j)->equals(i_->get(j)));
    }
    delete i;
    // check that the clone retains information/elements after original is deleted
    for (size_t j = 0; j < 10000; j++)
    {
        c = new char(all->at(j % size_));
        expected = new String(c);
        assert(i_->get(j)->equals(expected));
    }
    delete c;
    delete expected;
}

/*****************************************************************************************************************
 * testing dataframes
 **/

/**
 * @brief Getting the schema from a Dataframe. Testing if it has the expected row/columns as well as the 
 * elements.
 * 
 */
void testGetSchema1()
{
    // empty schema, no cols/rows
    Schema *s = new Schema();
    DataFrame *df = new DataFrame(*s);
    Schema got = df->get_schema();
    assert(got.width() == s->width());

    // nonempty schema, with only cols
    Schema *s2 = new Schema("IFIBS");
    DataFrame *df2 = new DataFrame(*s2);
    Schema got2 = df2->get_schema();
    assert(got2.width() == s2->width());
    for (size_t i = 0; i < df2->ncols(); i++)
        assert(got2.col_type(i) == s2->col_type(i));
}

/**
 * @brief Testing adding a column and then testing the dataframe's schema has changed as expected. 
 * Due to how this test is structured, we also can also use this test to test nrows() and ncols() functions. 
 * 
 */
void testAddColumn2()
{
    // empty schema, no cols/rows
    Schema *s = new Schema();
    DataFrame *df = new DataFrame(*s);
    assert(df->nrows() == 0);
    assert(df->ncols() == 0);
    Column *c = new IntColumn();
    for (int j = 1; j < 9; j++)
    {
        c->push_back(j);
    }
    df->add_column(c);
    assert(df->get_schema().width() == 1);
    assert(df->get_schema().col_type(0) == 'I');
    assert(df->nrows() == 8);
    assert(df->ncols() == 1);
    // now adding another column to nonempty dfs
    Column *cc = new BoolColumn();
    for (int j = 0; j < 8; j++)
    {
        cc->push_back(j % 2 == 0);
    }
    df->add_column(cc);
    assert(df->get_schema().width() == 2);
    assert(df->get_schema().col_type(1) == 'B');
    assert(df->nrows() == 8);
    assert(df->ncols() == 2);
}

/**
 * @brief Testing adding row by seeing if the dataframe's schema and other values have changed as expected. 
 * 
 */
void testAddRow3()
{
    // adding row to a nonempty schema
    Schema *s = new Schema("IISFB");    // nonempty schema
    DataFrame *df = new DataFrame(*s);  // make df based on nonempty schema
    Row *r = new Row(df->get_schema()); // making a row based off the schema
    // filling df with rows
    for (size_t i = 0; i < 1000; i++)
    {
        r->set(0, (int)i);
        r->set(1, (int)i + 1);
        r->set(2, new String("string hello"));
        r->set(3, (float)(i + 0.22));
        r->set(4, i % 2 == 1);
        df->add_row(*r);
    }
    assert(df->get_schema().width() == 5);
    assert(df->nrows() == 1000);
    assert(df->ncols() == 5);
}

/**
 * @brief Checking that in a nonempty 
 * 
 */
void testGet4()
{
    // empty schema, no cols/rows
    Schema *s = new Schema();
    DataFrame *df = new DataFrame(*s);
    // making columns to add
    Column *c1 = new IntColumn();
    Column *c2 = new BoolColumn();
    Column *c3 = new FloatColumn();
    Column *c4 = new StringColumn();

    String *all = new String("helloworld");
    String *toAdd;
    char *c;
    for (int j = 1; j < 9; j++)
    {
        c1->push_back(j);
        c2->push_back((j - 1) % 2 == 0);
        c3->push_back((float)(j + 0.22));
        c = new char(all->at(j - 1));
        toAdd = new String(c);
        c4->push_back(toAdd->clone());
        delete c;
        delete toAdd;
    }

    // adding columns to dfs
    df->add_column(c1);
    df->add_column(c2);
    df->add_column(c3);
    df->add_column(c4);

    assert(df->get_schema().coltypes_->equals((new String("IBFS"))));
    String *expected;
    char *ch;
    for (int i = 0; i < 8; i++)
    {
        assert(df->get_int(0, i) == (i + 1));
        assert(df->get_bool(1, i) == (i % 2 == 0));
        assert(df->get_float(2, i) - ((float)i + 1.22) < 0.001);
        assert(df->get_float(2, i) - ((float)i + 1.22) > -0.001);
        ch = new char(all->at(i));
        expected = new String(ch);
        assert(df->get_string(3, i)->equals(expected));
        delete expected;
        delete ch;
    }
}

/**
 * @brief Setting the different cells and then checking if we can get the expected value.
 * 
 */
void testSet5()
{
    // empty schema, no cols/rows
    Schema *s = new Schema();
    DataFrame *df = new DataFrame(*s);

    // making columns to add
    Column *c1 = new IntColumn();
    Column *c2 = new BoolColumn();
    Column *c3 = new FloatColumn();
    Column *c4 = new StringColumn();

    String *all = new String("helloworld");
    String *toAdd;
    char *c;
    for (int j = 1; j < 9; j++)
    {
        c1->push_back(j);
        c2->push_back((j - 1) % 2 == 0);
        c3->push_back((float)(j + 0.22));
        c = new char(all->at(j - 1));
        toAdd = new String(c);
        c4->push_back(toAdd->clone());
        delete c;
        delete toAdd;
    }

    // adding columns to dfs
    df->add_column(c1);
    df->add_column(c2);
    df->add_column(c3);
    df->add_column(c4);

    assert(df->get_schema().coltypes_->equals((new String("IBFS"))));

    char *ch;
    for (int i = 0; i < 8; i++)
    {
        df->set(0, i, (int)i * 2);
        df->set(1, i, i % 2 == 1);
        df->set(2, i, (float)(i + 2.22));
        ch = new char(all->at(i + 1));
        toAdd = new String(ch);
        df->set(3, i, toAdd);
        delete ch;
        delete toAdd;
    }
    String *expected;
    for (int i = 0; i < 8; i++)
    {
        assert(df->get_int(0, i) == (i * 2));
        assert(df->get_bool(1, i) == (i % 2 == 1));
        assert(df->get_float(2, i) - ((float)i + 2.22) < 0.001);
        assert(df->get_float(2, i) - ((float)i + 2.22) > -0.001);
        ch = new char(all->at(i + 1));
        expected = new String(ch);
        assert(df->get_string(3, i)->equals(expected));
        delete ch;
        delete expected;
    }
}

/**
 * @brief Runs all Tests in this file
 */
int main(int argc, char **argv)
{
    testGet();
    testSet();
    testGrowAndPushBack();
    testAddColumn();
    testAddEmptyNameColumn();
    testAddColToEmptySchemaNoName();
    testAddColToEmptySchema();
    testAcceptNonemptyRow();
    testAcceptEmptyRow();
    testIntColumnGet1();
    testIntColumnSet2();
    testIntColumnPushBack3();
    testIntColumnSize4();
    testIntColumnAsType5();
    testIntColumnGetType6();
    testIntColumnClone7();
    testBoolColumnGet1();
    testBoolColumnSet2();
    testBoolColumnPushBack3();
    testBoolColumnSize4();
    testBoolColumnAsType5();
    testBoolColumnGetType6();
    testBoolColumnClone7();
    testFloatColumnGet1();
    testFloatColumnSet2();
    testFloatColumnPushBack3();
    testFloatColumnSize4();
    testFloatColumnAsType5();
    testFloatColumnGetType6();
    testFloatColumnClone7();
    testStringColumnGet1();
    testStringColumnSet2();
    testStringColumnPushBack3();
    testStringColumnSize4();
    testStringColumnAsType5();
    testStringColumnGetType6();
    testStringColumnClone7();
    testGetSchema1();
    testAddColumn2();
    testAddRow3();
    testGet4();
    testSet5();

    std::cout << "All dataframe tests passed!" << std::endl;

    return 0;
}