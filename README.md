
[![GCC](https://github.com/ToyotaResearchInstitute/maliput_object/actions/workflows/build.yml/badge.svg)](https://github.com/ToyotaResearchInstitute/maliput_object/actions/workflows/build.yml)

# Maliput_object

Runtime interface for performing spacial queries on Maliput.

## Build

1. Setup a development workspace as described [here](https://shiny-fiesta-6b790eb7.pages.github.io/installation_quickstart.html).

2. Bring up your development workspace:

```sh
cd path/to/my/workspace
source ./bringup
```

3. Build maliput packages and their dependencies:

  - If not building drake from source:

   ```sh
   colcon build --packages-up-to maliput_object
   ```

  **Note**: To build documentation a `-BUILD_DOCS` cmake flag is required:
  ```sh
  colcon build --packages-select maliput_object --cmake-args " -DBUILD_DOCS=On"
  ```
