import * as chokidar from 'chokidar';
import { spawn } from 'child_process';

type Options = {
    capture_stdout: boolean;
    capture_stderr: boolean;
    capture_stdin: boolean;
    capture_all: boolean;
    command: string;
    timeout: number;
    delay: number;
    max_restarts: number;
    signal: string;
    verbose: boolean;
    quiet: boolean;
    fire_on_start: boolean;
};

let reload_count = 0;
let fired_on_start = false;

export function start_watching(directory_to_watch: string, command_to_run: string, exclude_patterns: RegExp[], options: Options): void {
    const watcher = chokidar.watch(directory_to_watch, {
        ignored: exclude_patterns,
        persistent: true,
        interval: options.delay ? options.delay * 1000 : 100,
        usePolling: true,
    });

    let cpp_process: any = null; // Store the C++ process reference

    // Reload function to execute when a file changes
    const reload = (): void => {
        console.log('Reloading C++ code...');

        // Stop the C++ application if it is running
        if (cpp_process !== null) {

            if (options.signal)
                cpp_process.kill(options.signal);
            else
                cpp_process.kill();

            cpp_process = null;
        }

        // Start the C++ application again
        cpp_process = spawn(command_to_run, [], {
            shell: true,
            stdio: 'inherit',
            detached: true,
            cwd: directory_to_watch,
            timeout: options.timeout || 5 * 60 * 1000,
        });

        cpp_process.on('exit', (code: number, signal: string) => {
            console.log(`C++ application exited with code ${code} and signal ${signal}`);
        });

        console.log('\x1b[32mC++ application started successfully\x1b[0m'); // Set text color to green (ANSI escape code)
    };

    // Watch for changes
    watcher.on('change', (path) => {
        console.log(`File ${path} has changed`);

        // check if previous process is still running
        if (cpp_process !== null) {
            cpp_process.kill();
            cpp_process = null;
        }

        // If the options specified a delay, resolve a promise after the delay
        if (options.delay) {
            return new Promise((resolve) => {
                setTimeout(resolve, options.delay * 1000);
            }).then(() => {
                reload();
            });
        } else {
            reload();
        }


        reload_count++;
        console.log(`Reload count: ${reload_count}`);
        if (reload_count > options.max_restarts) {
            console.log(`Maximum number of restarts reached. Exiting...`);
            process.exit(0);
        }
    });

    console.log(`Watching files in ${directory_to_watch}...`);
    if (options.fire_on_start && !fired_on_start) {
        fired_on_start = true;
        reload();
    }
}
