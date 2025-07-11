---

name: Bug Report
about: Found a bug? Report it
title: "[Bug] "
labels: Bug
assignees: tajemniktv

---

body:
  # ——— Intro ———
  - type: markdown
    attributes:
      value: |
        ## Thanks for taking the time to help!
        Please **search existing issues** before opening a new one.  
        The more details you provide, the faster we can reproduce and fix the problem.

  # ——— Mod version ———
  - type: input
    id: mod_version
    attributes:
      label: Mod version
      description: Full version string of the mod you're running.
      placeholder: "e.g. v1.2.3"
    validations:
      required: true

  # ——— Bug description ———
  - type: textarea
    id: description
    attributes:
      label: Describe the bug
      description: A clear and concise description of what went wrong.
      placeholder: "What happened? What did you expect to happen?"
    validations:
      required: true

  # ——— Steps to reproduce ———
  - type: textarea
    id: reproduction_steps
    attributes:
      label: Steps to reproduce
      description: Step-by-step instructions that trigger the bug.
      placeholder: |
        1. Start the game
        2. Load save “XYZ”
        3. …
      render: bash
    validations:
      required: true

  # ——— Expected behaviour ———
  - type: textarea
    id: expected
    attributes:
      label: Expected behaviour
      description: Tell us what you expected to happen instead.

  # ——— Screens / video ———
  - type: textarea
    id: media
    attributes:
      label: Screenshots / video
      description: Drag & drop images or link to a short video/GIF that shows the issue.

  # ——— Environment ———
  - type: textarea
    id: environment
    attributes:
      label: Environment
      description: System information that might be relevant.
      value: |
        - OS:
        - GPU:
        - Game version:
        - Other installed mods:
    validations:
      required: true

  # ——— Frequency ———
  - type: dropdown
    id: frequency
    attributes:
      label: How often does the bug occur?
      options:
        - Every launch
        - Often ( > 50 %)
        - Sometimes
        - Rarely
        - Once
    validations:
      required: true

  # ——— Confirmations ———
  - type: checkboxes
    id: confirmations
    attributes:
      label: Confirmations
      options:
        - label: I have searched existing issues to avoid duplicates.
          required: true
        - label: I am using the **latest** version of the mod and the game.
          required: true
        - label: I have attached all necessary files (logs, config, save files).
