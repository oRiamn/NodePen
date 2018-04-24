{
  "targets": [
    {
      "target_name": "XinputAddon",
      "sources": [ "./XinputAddon/main.cpp"],
      "include_dirs": [
      ],
      "libraries": [ 
        "-lX11",
        "-lXi",
        "-lm",
        "-lpthread"
       ]
    }
  ]
}