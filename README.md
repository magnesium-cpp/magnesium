Magnesium (abbreviated to MG - my initials) is a utility library for generic functionality that I have found useful to package in a single place so that I can use it in multiple projects easily. Consider it like a personal boost library. Boost is far more advanced, robust, or has very advanced libraries that could completely stand on their own. This is more focused on providing glue like code for things that I found myself writing often, could not find a good reusable implementation for, or felt like I had a novel small solution for.

Examples of the utilities provided by the library are provided below.

### std::tuple
- map
- zip
- runtime indexed operations

### functional
- chunk variadic parameters by some constant (provides for-loop like functionality and avoids tedious recursive formulation)
- functor which provides template provided constant when called
- functor which wraps template valid callable

### types
- check if a type is an implementation of a templated class (whose template only takes type parameters)

### error handling
- terse error handling 
- resilient retry of a flaky operation

Other examples that I would like to add but have not had the time to yet would be
- Random number provider (seeding is more difficult than it seems and can be verbose)
- Lazy evaluation wrapper type
- Fixed size, runtime defined sequence containers

Note that at this point the main consumer and developer is myself which means the feature set will be heavily influenced by my experience. Suggestions and contributions are welcome to include new features.