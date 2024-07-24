# Security Policy

## Safety and Stability

While the Chip8 Emulator is designed to be robust, running untrusted or malformed programs within the emulator can lead to undefined behavior. The most severe outcome of such behavior is typically a segmentation fault (segfault) due to invalid memory access in the C program. 

### Key Points

- **Undefined Behavior**: Running programs that are not well-formed or are intended to exploit vulnerabilities may result in unpredictable behavior.
- **Segmentation Fault**: In the worst case, the emulator may crash due to segmentation faults. This is a consequence of accessing invalid memory locations.

## Reporting Issues

If you encounter any issues or have concerns regarding the emulator’s stability, please follow these steps:

1. **Submit an Issue**: Open an issue on the GitHub repository with a detailed description of the problem. Include any relevant information about how to reproduce the issue.

2. **Provide Details**: When reporting, provide as much detail as possible, including:
   - The specific program or input that caused the issue.
   - The behavior observed (e.g., crash logs, error messages).

## Best Practices

- **Use Trusted Programs**: To avoid crashes and undefined behavior, ensure that the programs you run within the emulator are from trusted sources and are correctly formatted.
- **Update Regularly**: Keep your emulator up-to-date to benefit from improvements and bug fixes.

## Code of Conduct

Please follow our [Code of Conduct](CODE_OF_CONDUCT.md) when interacting with the project and reporting issues.

## Questions?

For further questions or concerns, please contact [my email](mailto:wojciech.linowski.2137@gmail.com).

Thank you for your understanding and cooperation!

---

