return {
    application = {
        config = {
            name = "SandboxApp",
            description = "A sandbox application for testing Ember Engine features.",
            version = "0.1.0",
            author = "Michael Smith",
        },
        run = {
            main = "main.lua",
            target_fps = 60,
        },
    },
    display = {
        window = {
            viewport_width = 800,
            viewport_height = 600,
            fullscreen = false,
            resizable = true,
            vsync = true,
            title = "SandboxApp",
        }
    }

}