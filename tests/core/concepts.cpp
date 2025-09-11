#include "GSE/core/concepts.hpp"

#include <string> // string

// clang-format off

// =====================
// --- Test typedefs ---
// =====================

constexpr gse::Extent M = 7;
constexpr gse::Extent N = 5;

using DoubleVector      = gse::Vector<double              >;
using FloatVector       = gse::Vector< float              >;
using IntVector         = gse::Vector<   int              >;
using FixedDoubleVector = gse::Vector<double, M           >;
using FixedFloatVector  = gse::Vector< float, M           >;
using FixedIntVector    = gse::Vector<   int, M           >;
using SmallDoubleVector = gse::Vector<double, M - 1       >;
using SmallFloatVector  = gse::Vector< float, M - 1       >;
using SmallIntVector    = gse::Vector<   int, M - 1       >;
using BigDoubleVector   = gse::Vector<double, M + 1       >;
using BigFloatVector    = gse::Vector< float, M + 1       >;
using BigIntVector      = gse::Vector<   int, M + 1       >;

using DoubleMatrix      = gse::Matrix<double              >;
using FloatMatrix       = gse::Matrix< float              >;
using IntMatrix         = gse::Matrix<   int              >;
using FixedDoubleMatrix = gse::Matrix<double, M    , N    >;
using FixedFloatMatrix  = gse::Matrix< float, M    , N    >;
using FixedIntMatrix    = gse::Matrix<   int, M    , N    >;
using SmallDoubleMatrix = gse::Matrix<double, M - 1, N - 1>;
using SmallFloatMatrix  = gse::Matrix< float, M - 1, N - 1>;
using SmallIntMatrix    = gse::Matrix<   int, M - 1, N - 1>;
using BigDoubleMatrix   = gse::Matrix<double, M + 1, N + 1>;
using BigFloatMatrix    = gse::Matrix< float, M + 1, N + 1>;
using BigIntMatrix      = gse::Matrix<   int, M + 1, N + 1>;

// ===========================
// --- Generic constraints ---
// ===========================

static_assert( gse::convertible_to<DoubleVector     , FixedFloatVector>); // dynamic -> static
static_assert( gse::convertible_to<FloatVector      , FixedFloatVector>);
static_assert( gse::convertible_to<IntVector        , FixedFloatVector>);
static_assert( gse::convertible_to<FixedDoubleVector, FloatVector     >); // static -> dynamic
static_assert( gse::convertible_to<FixedFloatVector , FloatVector     >);
static_assert( gse::convertible_to<FixedIntVector   , FloatVector     >);
static_assert(!gse::convertible_to<SmallDoubleVector, FixedFloatVector>); // static dimensions don't match
static_assert(!gse::convertible_to<SmallFloatVector , FixedFloatVector>);
static_assert(!gse::convertible_to<SmallIntVector   , FixedFloatVector>);
static_assert(!gse::convertible_to<BigDoubleVector  , FixedFloatVector>);
static_assert(!gse::convertible_to<BigFloatVector   , FixedFloatVector>);
static_assert(!gse::convertible_to<BigIntVector     , FixedFloatVector>);

static_assert( gse::convertible_to<DoubleMatrix     , FixedFloatMatrix>); // dynamic -> static
static_assert( gse::convertible_to<FloatMatrix      , FixedFloatMatrix>);
static_assert( gse::convertible_to<IntMatrix        , FixedFloatMatrix>);
static_assert( gse::convertible_to<FixedDoubleMatrix,      FloatMatrix>); // static -> dynamic
static_assert( gse::convertible_to<FixedFloatMatrix ,      FloatMatrix>);
static_assert( gse::convertible_to<FixedIntMatrix   ,      FloatMatrix>);
static_assert(!gse::convertible_to<SmallDoubleMatrix, FixedFloatMatrix>); // static dimensions don't match
static_assert(!gse::convertible_to<SmallFloatMatrix , FixedFloatMatrix>);
static_assert(!gse::convertible_to<SmallIntMatrix   , FixedFloatMatrix>);
static_assert(!gse::convertible_to<BigDoubleMatrix  , FixedFloatMatrix>);
static_assert(!gse::convertible_to<BigFloatMatrix   , FixedFloatMatrix>);
static_assert(!gse::convertible_to<BigIntMatrix     , FixedFloatMatrix>);

// ========================
// --- Type constraints ---
// ========================

static_assert( gse::scalar<         short    >);
static_assert( gse::scalar<         int      >);
static_assert( gse::scalar<         long     >);
static_assert( gse::scalar<         long long>);
static_assert( gse::scalar<unsigned short    >);
static_assert( gse::scalar<unsigned int      >);
static_assert( gse::scalar<unsigned long     >);
static_assert( gse::scalar<unsigned long long>);
static_assert( gse::scalar<float             >);
static_assert( gse::scalar<double            >);
static_assert( gse::scalar<long double       >);
static_assert(!gse::scalar<class Anonymous   >);
static_assert(!gse::scalar<double*           >);
static_assert(!gse::scalar<std::string       >);

static_assert( gse::vector<DoubleVector     >);
static_assert( gse::vector<FloatVector      >);
static_assert( gse::vector<IntVector        >);
static_assert( gse::vector<FixedDoubleVector>);
static_assert( gse::vector<FixedFloatVector >);
static_assert( gse::vector<FixedIntVector   >);
static_assert( gse::vector<SmallDoubleVector>);
static_assert( gse::vector<SmallFloatVector >);
static_assert( gse::vector<SmallIntVector   >);
static_assert( gse::vector<BigDoubleVector  >);
static_assert( gse::vector<BigFloatVector   >);
static_assert( gse::vector<BigIntVector     >);
static_assert(!gse::vector<DoubleMatrix     >);
static_assert(!gse::vector<FloatMatrix      >);
static_assert(!gse::vector<IntMatrix        >);
static_assert(!gse::vector<FixedDoubleMatrix>);
static_assert(!gse::vector<FixedFloatMatrix >);
static_assert(!gse::vector<FixedIntMatrix   >);
static_assert(!gse::vector<SmallDoubleMatrix>);
static_assert(!gse::vector<SmallFloatMatrix >);
static_assert(!gse::vector<SmallIntMatrix   >);
static_assert(!gse::vector<BigDoubleMatrix  >);
static_assert(!gse::vector<BigFloatMatrix   >);
static_assert(!gse::vector<BigIntMatrix     >);

static_assert( gse::matrix<DoubleVector     >);
static_assert( gse::matrix<FloatVector      >);
static_assert( gse::matrix<IntVector        >);
static_assert( gse::matrix<FixedDoubleVector>);
static_assert( gse::matrix<FixedFloatVector >);
static_assert( gse::matrix<FixedIntVector   >);
static_assert( gse::matrix<SmallDoubleVector>);
static_assert( gse::matrix<SmallFloatVector >);
static_assert( gse::matrix<SmallIntVector   >);
static_assert( gse::matrix<BigDoubleVector  >);
static_assert( gse::matrix<BigFloatVector   >);
static_assert( gse::matrix<BigIntVector     >);
static_assert( gse::matrix<DoubleMatrix     >);
static_assert( gse::matrix<FloatMatrix      >);
static_assert( gse::matrix<IntMatrix        >);
static_assert( gse::matrix<FixedDoubleMatrix>);
static_assert( gse::matrix<FixedFloatMatrix >);
static_assert( gse::matrix<FixedIntMatrix   >);
static_assert( gse::matrix<SmallDoubleMatrix>);
static_assert( gse::matrix<SmallFloatMatrix >);
static_assert( gse::matrix<SmallIntMatrix   >);
static_assert( gse::matrix<BigDoubleMatrix  >);
static_assert( gse::matrix<BigFloatMatrix   >);
static_assert( gse::matrix<BigIntMatrix     >);
static_assert(!gse::matrix<class Anonymous  >);
static_assert(!gse::matrix<double*          >);
static_assert(!gse::matrix<std::string      >);

// ========================
// --- Size constraints ---
// ========================

static_assert( gse::rows_equal_to<DoubleVector     , gse::dynamic>);
static_assert( gse::rows_equal_to<FloatVector      , gse::dynamic>);
static_assert( gse::rows_equal_to<IntVector        , gse::dynamic>);
static_assert( gse::rows_equal_to<FixedDoubleVector, M           >);
static_assert( gse::rows_equal_to<FixedFloatVector , M           >);
static_assert( gse::rows_equal_to<FixedIntVector   , M           >);
static_assert( gse::rows_equal_to<SmallDoubleVector, M - 1       >);
static_assert( gse::rows_equal_to<SmallFloatVector , M - 1       >);
static_assert( gse::rows_equal_to<SmallIntVector   , M - 1       >);
static_assert( gse::rows_equal_to<BigDoubleVector  , M + 1       >);
static_assert( gse::rows_equal_to<BigFloatVector   , M + 1       >);
static_assert( gse::rows_equal_to<BigIntVector     , M + 1       >);
static_assert( gse::rows_equal_to<DoubleMatrix     , gse::dynamic>);
static_assert( gse::rows_equal_to<FloatMatrix      , gse::dynamic>);
static_assert( gse::rows_equal_to<IntMatrix        , gse::dynamic>);
static_assert( gse::rows_equal_to<FixedDoubleMatrix, M           >);
static_assert( gse::rows_equal_to<FixedFloatMatrix , M           >);
static_assert( gse::rows_equal_to<FixedIntMatrix   , M           >);
static_assert( gse::rows_equal_to<SmallDoubleMatrix, M - 1       >);
static_assert( gse::rows_equal_to<SmallFloatMatrix , M - 1       >);
static_assert( gse::rows_equal_to<SmallIntMatrix   , M - 1       >);
static_assert( gse::rows_equal_to<BigDoubleMatrix  , M + 1       >);
static_assert( gse::rows_equal_to<BigFloatMatrix   , M + 1       >);
static_assert( gse::rows_equal_to<BigIntMatrix     , M + 1       >);
static_assert(!gse::rows_equal_to<class Anonymous  , gse::dynamic>);
static_assert(!gse::rows_equal_to<double*          , gse::dynamic>);
static_assert(!gse::rows_equal_to<std::string      , gse::dynamic>);

static_assert( gse::cols_equal_to<DoubleVector     , 1           >);
static_assert( gse::cols_equal_to<FloatVector      , 1           >);
static_assert( gse::cols_equal_to<IntVector        , 1           >);
static_assert( gse::cols_equal_to<FixedDoubleVector, 1           >);
static_assert( gse::cols_equal_to<FixedFloatVector , 1           >);
static_assert( gse::cols_equal_to<FixedIntVector   , 1           >);
static_assert( gse::cols_equal_to<SmallDoubleVector, 1           >);
static_assert( gse::cols_equal_to<SmallFloatVector , 1           >);
static_assert( gse::cols_equal_to<SmallIntVector   , 1           >);
static_assert( gse::cols_equal_to<BigDoubleVector  , 1           >);
static_assert( gse::cols_equal_to<BigFloatVector   , 1           >);
static_assert( gse::cols_equal_to<BigIntVector     , 1           >);
static_assert( gse::cols_equal_to<DoubleMatrix     , gse::dynamic>);
static_assert( gse::cols_equal_to<FloatMatrix      , gse::dynamic>);
static_assert( gse::cols_equal_to<IntMatrix        , gse::dynamic>);
static_assert( gse::cols_equal_to<FixedDoubleMatrix, N           >);
static_assert( gse::cols_equal_to<FixedFloatMatrix , N           >);
static_assert( gse::cols_equal_to<FixedIntMatrix   , N           >);
static_assert( gse::cols_equal_to<SmallDoubleMatrix, N - 1       >);
static_assert( gse::cols_equal_to<SmallFloatMatrix , N - 1       >);
static_assert( gse::cols_equal_to<SmallIntMatrix   , N - 1       >);
static_assert( gse::cols_equal_to<BigDoubleMatrix  , N + 1       >);
static_assert( gse::cols_equal_to<BigFloatMatrix   , N + 1       >);
static_assert( gse::cols_equal_to<BigIntMatrix     , N + 1       >);
static_assert(!gse::cols_equal_to<class Anonymous  , gse::dynamic>);
static_assert(!gse::cols_equal_to<double*          , gse::dynamic>);
static_assert(!gse::cols_equal_to<std::string      , gse::dynamic>);

// ============================
// --- Function constraints ---
// ============================

double                double_to_double(               double x) { return x; }
double                double_to_float (               double x) { return x; }
int                   void_to_int     (                       ) { return 0; }
void                  double_to_void  (               double  ) {           }
gse::Vector<float, N> vector_to_vector(gse::Vector<float, N> x) { return x; }

static_assert( gse::scalar_function<decltype(&double_to_double), double              >);
static_assert( gse::scalar_function<decltype(&double_to_float ), double              >);
static_assert(!gse::scalar_function<decltype(&void_to_int     ), double              >);
static_assert(!gse::scalar_function<decltype(&double_to_void  ), double              >);
static_assert( gse::vector_function<decltype(&vector_to_vector), double, N           >);
static_assert( gse::vector_function<decltype(&vector_to_vector), double, gse::dynamic>);
static_assert(!gse::vector_function<decltype(&vector_to_vector), double, N + 1       >);
static_assert(!gse::vector_function<decltype(&vector_to_vector), double, N - 1       >);
// clang-format on