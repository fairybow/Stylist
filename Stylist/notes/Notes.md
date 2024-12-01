# Notes

## Todo

Roadmap

- Push R/D dll/lib to diff folders
- Edit window themes to use menu font color
- Reverse wondow theme order
- Add demo of using the variable in multiple places of template, not just one
- Go through and make any non-api function underscore-prefixed?
- Default args in QSS templates (for if a theme doesn't use a template arg--for now, they must match)
- Editable in-tool
- Go through and find private member functions that don't deal with member variables and move them into the CPP file as static above the class definitions
- Const/constexpr/noexcept checks!--check the status of qt functions within for clues
    - Start from the bottom up when searching for constexpr/noexcept
- Go through and look at all debug formats, check whether should be in cpp, should static, whatever
- Make style uniform (brackets, always use brackets?, exploding, etc--perhaps relent and use clang)
- Diagram update!
- Uniform debug functions
- Investigate all debug messages
    - Symbol loading disabled by Include/Exclude setting. = ???
    - PDB is in the output dir next to module/binary
- Potentially, organize headers like WidgetGroup (multiple public/pvt sections) and make CPP files alphabetical
- Alphabetize in the headers per-section
- Consider using JSON for settings
- For this project, no settings but need to make sure to offer API to store information
- Apply the style guidelines to Coco and eventually Fernanda
- Check your lambdas. Don't capture by value `=` unless local. Can use ref if a memvar
- Add a way to provide default arg to themes in template (if themes don't include that property)
- Add a default name? Take from meta object name?

---

If you see the following, you're running a debug consuming project with a release DLL or vice versa:

QWidget: Must construct a QApplication before a QWidget
Debug Error!
