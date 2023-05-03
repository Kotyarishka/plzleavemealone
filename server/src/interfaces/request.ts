import { Request } from "express";

export interface RequestWithLockId extends Request {
    lockId?: string;
}

export interface RequestWithAllLocks extends Request {
    locks?: Array<{ id: string, locked: boolean | undefined }>;
}
