# Visual Studio solution setup notes

(I forget things.)

> [!CAUTION]
> Remember to change repo in consuming test project script pre-build call (it's in there twice!). Remember to clean builds and let scripts build copy correct DLL.

## Language standard
```
ISO C++20 Standard (/std:c++20)
```

## Stylist

### Additional include directories

C/C++

```
$(ProjectDir)include\Stylist\;$(ProjectDir)src\;$(ProjectDir)submodules\Coco\include;%(AdditionalIncludeDirectories)
```

### Preprocessor definitions

C/C++ > Preprocessor

```
STYLIST_LIB;%(PreprocessorDefinitions)
```

## Demo

### Additional include directories

C/C++

```
$(SolutionDir)Stylist\include;%(AdditionalIncludeDirectories)
```

### Additional library directories

Linker

```
$(SolutionDir)Stylist\lib;%(AdditionalLibraryDirectories)
```

### Additional dependencies

Linker > Input

```
$(IntDir)Stylist.lib;$(CoreLibraryDependencies);%(AdditionalDependencies)
```

## Consumer Test

### Additional include directories

C/C++

```
$(ProjectDir)Stylist\include;%(AdditionalIncludeDirectories)
```

### Additional library directories

Linker

```
$(ProjectDir)Stylist\lib;%(AdditionalLibraryDirectories)
```

### Additional dependencies

Linker > Input

```
$(IntDir)Stylist.lib;$(CoreLibraryDependencies);%(AdditionalDependencies)
```
