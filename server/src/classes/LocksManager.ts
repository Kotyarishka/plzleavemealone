import StateManager from "./StateManager";

class LocksManager {
    private locks: StateManager;

    constructor() {
        this.locks = new StateManager();
    }

    /**
     * Adds a lock to the locks manager.
     * @param key The key of the lock.
     * @memberof LocksManager
     * @returns void
     * @throws Error if lock already exists.
     */
    public addLock(key: string): void {
        if (this.locks.hasKey(key)) {
            throw new Error("Lock already exists.");
        };

        this.locks.setState(key, true) // defaults to locked
    }

    /**
     * Removes a lock from the locks manager.
     * @param key The key of the lock.
     * @memberof LocksManager
     * @returns void
     * @throws Error if lock does not exist.
     */
    public removeLock(key: string): void {
        if (!this.locks.hasKey(key)) {
            throw new Error("Lock does not exist.");
        };

        this.locks.deleteKey(key);
    }

    /**
     * Checks if a lock is locked.
     * @param key The key of the lock.
     * @returns boolean
     * @memberof LocksManager
     * @returns boolean if lock is locked.
     * @throws Error if lock does not exist.
     */
    public isLocked(key: string): boolean {
        if (!this.locks.hasKey(key)) {
            throw new Error("Lock does not exist.");
        };

        return this.locks.getState(key) === true;
    }

    public getAllLocks(): Map<string, boolean | undefined> {
        return this.locks.getLocalState();
    }

    /**
     * Locks a lock.
     * @param key The key of the lock.
     * @memberof LocksManager
     * @returns void
     * @throws Error if lock does not exist.
     */
    public lock(key: string): void {
        if (!this.locks.hasKey(key)) {
            throw new Error("Lock does not exist.");
        };

        this.locks.setState(key, true);
    }

    /**
     * Unlocks a lock.
     * @param key The key of the lock.
     * @memberof LocksManager
     * @returns void
     * @throws Error if lock does not exist.
     */
    public unlock(key: string): void {
        if (!this.locks.hasKey(key)) {
            throw new Error("Lock does not exist.");
        };

        this.locks.setState(key, false);
    }

    /**
     * Checks if a lock exists.
     * @param key The key of the lock.
     * @memberof LocksManager
     * @returns boolean if lock exists.
     */
    public hasLock(key: string): boolean {
        return this.locks.hasKey(key);
    }

    /**
     * Gets the underlying StateManager.
     * @returns StateManager
     * @memberof LocksManager
     */
    public getStateManager(): StateManager {
        return this.locks;
    }
}

export default new LocksManager();