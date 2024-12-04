namespace build2
{
  namespace autoconf
  {
    struct check
    {
      const char* name;
      const char* modifier; // ! or empty
      const char* base;     // base names or empty
      const char* value;
    };

    extern const check checks[206];
  }
}
